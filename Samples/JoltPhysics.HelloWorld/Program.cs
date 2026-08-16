using Evergine.Bindings.JoltPhysics;
using Evergine.Common.Graphics;
using Evergine.Common.Graphics.VertexFormats;
using Evergine.Common.Input;
using Evergine.DirectX12;
using Evergine.Forms;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Buffer = Evergine.Common.Graphics.Buffer;
using Color = Evergine.Common.Graphics.Color;
using Keys = Evergine.Common.Input.Keyboard.Keys;
using Matrix4x4 = Evergine.Mathematics.Matrix4x4;
using Quaternion = Evergine.Mathematics.Quaternion;
using Rectangle = Evergine.Mathematics.Rectangle;
using Vector3 = Evergine.Mathematics.Vector3;
using Vector4 = Evergine.Mathematics.Vector4;

namespace JoltHelloWorld
{
	/// <summary>
	/// Windowed demo: JoltPhysics simulates 30 bodies dropped in three waves and the Evergine
	/// low-level graphics API draws every body into a Windows Forms window through a DirectX 12
	/// swap chain. Physics runs on a fixed step, decoupled from the rate the window presents at.
	///
	/// The scene mirrors the MuJoCo.NET LowLevelDemo one body for body. Jolt is Y-up where MuJoCo
	/// is Z-up, so positions and half extents are the same numbers with Y and Z swapped.
	/// </summary>
	unsafe class Program
	{
		private const uint Width = 1280;
		private const uint Height = 720;
		private const uint CbSlotSize = 256; // per-body constant slot, 256-byte aligned

		private const ushort LayerNonMoving = 0;
		private const ushort LayerMoving = 1;
		private const uint NumObjectLayers = 2;
		private const uint NumBroadPhaseLayers = 2;

		private const float TimeStep = 1f / 60f;

		/// <summary>A slow frame must not turn into a physics catch-up spiral.</summary>
		private const float MaxFrameTime = 0.25f;

		private const string Title = "JoltPhysics HelloWorld [DX12]";

		/// <summary>
		/// Release time per wave. MuJoCo compiles its model up front, so its demo has to park the
		/// later waves out of frame and pin them; Jolt lets bodies join a live world, so here the
		/// wave is simply not added until its time comes.
		/// </summary>
		private static readonly float[] WaveReleaseTime = { 0f, 2.0f, 4.2f };

		private enum Kind
		{
			Box,
			Sphere,
			Capsule,
		}

		/// <summary>
		/// Scene description, in MuJoCo's Z-up coordinates so it can be diffed against the MJCF in
		/// MuJoCo.NET/LowLevelDemo. Conversion to Jolt's Y-up happens in <see cref="ToJolt"/>.
		/// Box size is half extents, capsule size is (radius, half length of the cylinder) — the
		/// same conventions both engines use.
		/// </summary>
		private record struct Geom(int Wave, Kind Kind, Vector3 Size, Vector3 Pos, Vector3 EulerDeg, Vector3 Rgb);

		private static readonly Geom[] Scene =
		{
			// wave 1
			new(0, Kind.Box, new(0.22f, 0.22f, 0.22f), new( 0.00f,  0.00f, 1.2f), new(10, 25,  0), new(0.90f, 0.30f, 0.24f)),
			new(0, Kind.Box, new(0.18f, 0.18f, 0.18f), new( 0.42f,  0.18f, 1.7f), new( 0, 30, 40), new(0.16f, 0.50f, 0.73f)),
			new(0, Kind.Box, new(0.15f, 0.25f, 0.12f), new(-0.35f,  0.30f, 2.1f), new(20,  0, 65), new(0.95f, 0.61f, 0.07f)),
			new(0, Kind.Box, new(0.20f, 0.12f, 0.16f), new( 0.12f, -0.40f, 2.5f), new(45, 15, 10), new(0.10f, 0.74f, 0.61f)),
			new(0, Kind.Box, new(0.14f, 0.14f, 0.28f), new(-0.50f, -0.12f, 1.4f), new( 5, 50, 20), new(0.61f, 0.35f, 0.71f)),
			new(0, Kind.Sphere, new(0.18f, 0, 0), new( 0.60f, -0.32f, 1.9f), default, new(0.91f, 0.30f, 0.55f)),
			new(0, Kind.Sphere, new(0.14f, 0, 0), new(-0.18f,  0.58f, 2.4f), default, new(0.20f, 0.60f, 0.86f)),
			new(0, Kind.Sphere, new(0.21f, 0, 0), new( 0.28f,  0.48f, 1.5f), default, new(0.95f, 0.77f, 0.06f)),
			new(0, Kind.Capsule, new(0.10f, 0.25f, 0), new( 0.06f,  0.22f, 2.8f), new(80,  0, 30), new(0.90f, 0.49f, 0.13f)),
			new(0, Kind.Capsule, new(0.08f, 0.20f, 0), new(-0.28f, -0.50f, 2.2f), new(15, 70,  0), new(0.75f, 0.22f, 0.17f)),

			// wave 2
			new(1, Kind.Box, new(0.19f, 0.19f, 0.19f), new( 0.30f,  0.35f, 5.6f), new(30, 10, 55), new(0.20f, 0.29f, 0.37f)),
			new(1, Kind.Box, new(0.13f, 0.22f, 0.17f), new(-0.40f,  0.10f, 5.9f), new( 0, 45, 20), new(0.85f, 0.37f, 0.01f)),
			new(1, Kind.Box, new(0.24f, 0.14f, 0.14f), new( 0.15f, -0.45f, 6.2f), new(60,  0, 35), new(0.44f, 0.62f, 0.81f)),
			new(1, Kind.Box, new(0.16f, 0.16f, 0.24f), new(-0.10f,  0.50f, 5.7f), new(15, 35,  5), new(0.99f, 0.85f, 0.21f)),
			new(1, Kind.Sphere, new(0.16f, 0, 0), new( 0.52f, -0.05f, 6.4f), default, new(0.56f, 0.27f, 0.68f)),
			new(1, Kind.Sphere, new(0.20f, 0, 0), new(-0.55f, -0.30f, 6.0f), default, new(0.11f, 0.63f, 0.51f)),
			new(1, Kind.Sphere, new(0.13f, 0, 0), new( 0.05f,  0.05f, 6.6f), default, new(0.94f, 0.40f, 0.40f)),
			new(1, Kind.Capsule, new(0.09f, 0.22f, 0), new( 0.38f,  0.52f, 5.8f), new(70, 20,  0), new(0.25f, 0.55f, 0.79f)),
			new(1, Kind.Capsule, new(0.11f, 0.19f, 0), new(-0.22f, -0.15f, 6.3f), new(10, 80, 40), new(0.90f, 0.62f, 0.10f)),
			new(1, Kind.Capsule, new(0.07f, 0.26f, 0), new( 0.60f,  0.25f, 6.1f), new(45, 45,  0), new(0.36f, 0.72f, 0.36f)),

			// wave 3
			new(2, Kind.Box, new(0.21f, 0.15f, 0.15f), new(-0.05f,  0.40f, 5.6f), new(25, 55, 10), new(0.72f, 0.11f, 0.28f)),
			new(2, Kind.Box, new(0.17f, 0.17f, 0.21f), new( 0.45f, -0.38f, 5.9f), new( 0, 15, 70), new(0.13f, 0.44f, 0.62f)),
			new(2, Kind.Box, new(0.12f, 0.20f, 0.18f), new(-0.48f,  0.22f, 6.3f), new(50, 30, 25), new(0.96f, 0.71f, 0.13f)),
			new(2, Kind.Box, new(0.15f, 0.15f, 0.15f), new( 0.20f,  0.12f, 6.6f), new(35,  0, 45), new(0.31f, 0.66f, 0.44f)),
			new(2, Kind.Sphere, new(0.19f, 0, 0), new(-0.30f, -0.42f, 5.7f), default, new(0.86f, 0.29f, 0.62f)),
			new(2, Kind.Sphere, new(0.15f, 0, 0), new( 0.58f,  0.08f, 6.1f), default, new(0.29f, 0.56f, 0.90f)),
			new(2, Kind.Sphere, new(0.22f, 0, 0), new(-0.12f, -0.05f, 6.5f), default, new(0.98f, 0.80f, 0.30f)),
			new(2, Kind.Capsule, new(0.10f, 0.20f, 0), new( 0.10f,  0.55f, 6.0f), new(65, 10, 20), new(0.55f, 0.34f, 0.76f)),
			new(2, Kind.Capsule, new(0.08f, 0.24f, 0), new(-0.58f,  0.00f, 6.4f), new(20, 60, 50), new(0.87f, 0.45f, 0.20f)),
			new(2, Kind.Capsule, new(0.12f, 0.17f, 0), new( 0.32f, -0.20f, 5.8f), new(80, 30, 15), new(0.16f, 0.70f, 0.66f)),
		};

		/// <summary>MuJoCo is Z-up, Jolt is Y-up: same numbers, Y and Z swapped.</summary>
		private static Vector3 ToJolt(Vector3 zUp) => new(zUp.X, zUp.Z, zUp.Y);

		[StructLayout(LayoutKind.Sequential)]
		private struct PerObject
		{
			public Matrix4x4 WorldViewProj;
			public Matrix4x4 World;
			public Vector4 Color;
		}

		private class Mesh
		{
			public Buffer VertexBuffer;
			public Buffer IndexBuffer;
			public uint IndexCount;
		}

		private class Renderable
		{
			public uint BodyId;
			public bool Added;
			public int Wave;
			public Mesh Mesh;
			public Vector4 Color;

			/// <summary>Shape-space transform applied before the body's world transform.</summary>
			public Matrix4x4 Local;

			/// <summary>Spawn pose, kept so the drop can be replayed without restarting.</summary>
			public RVec3 InitialPosition;

			public Quat InitialRotation;
		}

		// ---- Jolt ------------------------------------------------------------------------------
		private static IntPtr physicsSystem;
		private static IntPtr bodyInterface;
		private static IntPtr tempAllocator;
		private static IntPtr jobSystem;
		private static IntPtr broadPhase;
		private static IntPtr objectLayerPairFilter;
		private static IntPtr objectVsBroadPhaseFilter;
		private static readonly List<IntPtr> shapes = new();

		// ---- Evergine low level ------------------------------------------------------------------
		private static GraphicsContext graphics;
		private static SwapChain swapChain;
		private static FrameBuffer frameBuffer;
		private static Window window;
		private static Surface surface;
		private static CommandQueue commandQueue;
		private static GraphicsPipelineState pipeline;
		private static ResourceSet resourceSet;
		private static Buffer constantBuffer;
		private static byte[] cbData;
		private static Viewport[] viewports;
		private static Rectangle[] scissors;
		private static Matrix4x4 view;
		private static Matrix4x4 proj;

		private static readonly Dictionary<string, Mesh> meshes = new();
		private static readonly List<Renderable> renderables = new();

		// ---- Timing ----------------------------------------------------------------------------
		private static readonly Stopwatch clock = new();
		private static readonly Stopwatch fpsTimer = new();
		private static float accumulator;
		private static float simulationTime;
		private static int fpsCounter;
		private static bool windowResized;

		[STAThread]
		static void Main()
		{
			InitializePhysics();

			// The window is a plain System.Windows.Forms.Form driven by Evergine's Forms window
			// system, which also owns the render loop and the input dispatchers.
			var windowsSystem = new FormsWindowsSystem();
			window = windowsSystem.CreateWindow(Title, Width, Height);
			surface = window;
			surface.OnScreenSizeChanged += (sender, args) => windowResized = true;

			graphics = new DX12GraphicsContext();
			graphics.CreateDevice(new ValidationLayer(ValidationLayer.NotifyMethod.Trace));

			var swapChainDescription = new SwapChainDescription()
			{
				SurfaceInfo = surface.SurfaceInfo,
				Width = surface.Width,
				Height = surface.Height,
				ColorTargetFormat = PixelFormat.R8G8B8A8_UNorm,
				ColorTargetFlags = TextureFlags.RenderTarget | TextureFlags.ShaderResource,
				DepthStencilTargetFormat = PixelFormat.D24_UNorm_S8_UInt,
				DepthStencilTargetFlags = TextureFlags.DepthStencil,
				SampleCount = TextureSampleCount.None,
				IsWindowed = true,
				RefreshRate = 60,
			};
			swapChain = graphics.CreateSwapChain(swapChainDescription);
			swapChain.VerticalSync = true;

			windowsSystem.Run(Load, Draw);

			Shutdown();
		}

		private static void InitializePhysics()
		{
			// Process-global, exactly once, in this order.
			JoltPhysics.RegisterDefaultAllocator();
			JoltPhysics.Init();
			JoltPhysics.CreateFactory();
			JoltPhysics.RegisterTypes();

			tempAllocator = JoltPhysics.TempAllocator_Create(10 * 1024 * 1024);
			jobSystem = JoltPhysics.JobSystemThreadPool_Create(2048, 8, 1);

			// Layer tables. The ObjectVsBroadPhase table is derived from the other two, so it has
			// to be built after the mappings and the enabled pairs are in place — getting this
			// wrong is the classic "nothing collides" bug.
			broadPhase = JoltPhysics.BroadPhaseLayerInterfaceTable_Create(NumObjectLayers, NumBroadPhaseLayers);
			JoltPhysics.BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(broadPhase, LayerNonMoving, 0);
			JoltPhysics.BroadPhaseLayerInterfaceTable_MapObjectToBroadPhaseLayer(broadPhase, LayerMoving, 1);

			objectLayerPairFilter = JoltPhysics.ObjectLayerPairFilterTable_Create(NumObjectLayers);
			JoltPhysics.ObjectLayerPairFilterTable_EnableCollision(objectLayerPairFilter, LayerNonMoving, LayerMoving);
			JoltPhysics.ObjectLayerPairFilterTable_EnableCollision(objectLayerPairFilter, LayerMoving, LayerMoving);

			objectVsBroadPhaseFilter = JoltPhysics.ObjectVsBroadPhaseLayerFilterTable_Create(
				broadPhase, NumBroadPhaseLayers, objectLayerPairFilter, NumObjectLayers);

			const uint maxBodies = 1024;
			physicsSystem = JoltPhysics.PhysicsSystem_Create();
			JoltPhysics.PhysicsSystem_Init(
				physicsSystem, maxBodies, Math.Max(1u, maxBodies / 64), 1024, 1024,
				broadPhase, objectVsBroadPhaseFilter, objectLayerPairFilter);

			bodyInterface = JoltPhysics.PhysicsSystem_GetBodyInterface(physicsSystem);
			JoltPhysics.PhysicsSystem_SetGravity(physicsSystem, new Vec3 { X = 0, Y = -9.81f, Z = 0 });
		}

		private static void Load()
		{
			frameBuffer = swapChain.FrameBuffer;

			var vsBytes = graphics.ShaderCompile(Shaders.Hlsl, "VS", ShaderStages.Vertex).ByteCode;
			var psBytes = graphics.ShaderCompile(Shaders.Hlsl, "PS", ShaderStages.Pixel).ByteCode;
			var vsDescription = new ShaderDescription(ShaderStages.Vertex, "VS", vsBytes);
			var psDescription = new ShaderDescription(ShaderStages.Pixel, "PS", psBytes);
			var vertexShader = graphics.Factory.CreateShader(ref vsDescription);
			var pixelShader = graphics.Factory.CreateShader(ref psDescription);

			// ---- Bodies -----------------------------------------------------------------------
			CreateGround();
			foreach (var geom in Scene)
			{
				CreateBody(geom);
			}

			JoltPhysics.PhysicsSystem_OptimizeBroadPhase(physicsSystem);

			int slots = renderables.Count;
			cbData = new byte[CbSlotSize * slots];

			var cbDescription = new BufferDescription(
				CbSlotSize * (uint)slots, BufferFlags.ConstantBuffer, ResourceUsage.Default);
			constantBuffer = graphics.Factory.CreateBuffer(ref cbDescription);

			var layoutDescription = new ResourceLayoutDescription(
				new LayoutElementDescription(0, ResourceType.ConstantBuffer,
					ShaderStages.Vertex | ShaderStages.Pixel, allowDynamicOffset: true, size: CbSlotSize));
			var resourceLayout = graphics.Factory.CreateResourceLayout(ref layoutDescription);

			var resourceSetDescription = new ResourceSetDescription(resourceLayout, constantBuffer);
			resourceSet = graphics.Factory.CreateResourceSet(ref resourceSetDescription);

			var pipelineDescription = new GraphicsPipelineDescription()
			{
				PrimitiveTopology = PrimitiveTopology.TriangleList,
				InputLayouts = new InputLayouts().Add(VertexPositionNormalTangentTexture.VertexFormat),
				ResourceLayouts = new[] { resourceLayout },
				Shaders = new GraphicsShaderStateDescription()
				{
					VertexShader = vertexShader,
					PixelShader = pixelShader,
				},
				RenderStates = new RenderStateDescription()
				{
					RasterizerState = RasterizerStates.CullBack,
					BlendState = BlendStates.Opaque,
					DepthStencilState = DepthStencilStates.ReadWrite,
				},
				Outputs = frameBuffer.OutputDescription,
			};
			pipeline = graphics.Factory.CreateGraphicsPipeline(ref pipelineDescription);
			commandQueue = graphics.Factory.CreateCommandQueue();

			// Same framing as the MuJoCo demo, expressed in Y-up.
			view = Matrix4x4.CreateLookAt(new Vector3(2.6f, 1.9f, -2.6f), new Vector3(0, 0.5f, 0), Vector3.UnitY);

			viewports = new Viewport[1];
			scissors = new Rectangle[1];
			ScreenSizeChanged(surface.Width, surface.Height);

			clock.Restart();
			fpsTimer.Restart();
		}

		private static void Draw()
		{
			surface.KeyboardDispatcher?.DispatchEvents();

			if (surface.KeyboardDispatcher?.ReadKeyState(Keys.R) == ButtonState.Releasing)
			{
				ResetScene();
			}

			if (windowResized)
			{
				windowResized = false;
				swapChain.ResizeSwapChain(surface.Width, surface.Height);
				frameBuffer = swapChain.FrameBuffer;
				ScreenSizeChanged(surface.Width, surface.Height);
			}

			swapChain.InitFrame();

			var elapsed = (float)clock.Elapsed.TotalSeconds;
			clock.Restart();
			UpdatePhysics(Math.Min(elapsed, MaxFrameTime));

			FillConstants(Matrix4x4.Multiply(view, proj));
			DrawFrame();

			swapChain.Present();

			UpdateTitle();
		}

		/// <summary>
		/// Fixed-step integration: the leftover time carries over to the next frame, so the
		/// simulation behaves the same no matter what rate the window presents at.
		/// </summary>
		private static void UpdatePhysics(float elapsed)
		{
			accumulator += elapsed;

			while (accumulator >= TimeStep)
			{
				ReleaseDueWaves(simulationTime);

				uint error = JoltPhysics.PhysicsSystem_Update(physicsSystem, TimeStep, 1, tempAllocator, jobSystem);
				if (error != 0)
				{
					Trace.WriteLine($"PhysicsSystem_Update returned {error} at t={simulationTime:F3}");
				}

				simulationTime += TimeStep;
				accumulator -= TimeStep;
			}
		}

		private static void ScreenSizeChanged(uint width, uint height)
		{
			viewports[0] = new Viewport(0, 0, width, height);
			scissors[0] = new Rectangle(0, 0, (int)width, (int)height);
			proj = Matrix4x4.CreatePerspectiveFieldOfView(
				Evergine.Mathematics.MathHelper.PiOver4, (float)width / height, 0.1f, 100f, reverseDepthBuffer: true);
		}

		/// <summary>The window caption doubles as the HUD: sim time, body counts and frame rate.</summary>
		private static void UpdateTitle()
		{
			fpsCounter++;
			if (fpsTimer.ElapsedMilliseconds <= 1000)
			{
				return;
			}

			float fps = 1000f * fpsCounter / fpsTimer.ElapsedMilliseconds;
			uint bodies = JoltPhysics.PhysicsSystem_GetNumBodies(physicsSystem);
			uint active = JoltPhysics.PhysicsSystem_GetNumActiveBodies(physicsSystem, BodyType.Rigid);

			window.Title = $"{Title}  t={simulationTime:F1}s  bodies={bodies}  active={active}  FPS: {fps:F1}  [R] restart";

			fpsTimer.Restart();
			fpsCounter = 0;
		}

		private static void CreateGround()
		{
			// MuJoCo uses an infinite plane; Jolt's well-trodden equivalent is a large thin static
			// box. Its top face sits at y = 0 in both.
			const float halfSize = 7f;
			const float halfThickness = 0.05f;

			var shape = JoltPhysics.BoxShape_Create(
				new Vec3 { X = halfSize, Y = halfThickness, Z = halfSize }, 0f);
			shapes.Add(shape);

			BodyCreationSettings settings = default;
			JoltPhysics.BodyCreationSettings_SetDefault(&settings);
			settings.Shape = shape;
			settings.MotionType = MotionType.Static;
			settings.ObjectLayer = LayerNonMoving;
			settings.Position = new RVec3 { X = 0, Y = -halfThickness, Z = 0 };
			settings.Rotation = new Quat { X = 0, Y = 0, Z = 0, W = 1 };

			uint bodyId = JoltPhysics.BodyInterface_CreateBody(bodyInterface, &settings);
			JoltPhysics.BodyInterface_AddBody(bodyInterface, bodyId, Activation.DontActivate);

			renderables.Add(new Renderable
			{
				BodyId = bodyId,
				Added = true,
				Wave = -1,
				Mesh = GetCubeMesh(),
				Color = new Vector4(0.72f, 0.70f, 0.66f, 1f),
				Local = Matrix4x4.CreateScale(halfSize * 2, halfThickness * 2, halfSize * 2),
				InitialPosition = settings.Position,
				InitialRotation = settings.Rotation,
			});
		}

		private static void CreateBody(Geom geom)
		{
			IntPtr shape;
			Mesh mesh;
			Matrix4x4 local;

			switch (geom.Kind)
			{
				case Kind.Sphere:
					float radius = geom.Size.X;
					shape = JoltPhysics.SphereShape_Create(radius);
					mesh = GetSphereMesh();
					local = Matrix4x4.CreateScale(radius * 2);
					break;

				case Kind.Capsule:
					// Both engines take (radius, half length of the cylinder); Jolt's argument order
					// is the other way round. Jolt capsules run along local Y, which is also the
					// axis Primitives.Capsule generates, so no extra rotation is needed here — the
					// MuJoCo side does need one because its capsules run along local Z.
					float capRadius = geom.Size.X;
					float capHalfLength = geom.Size.Y;
					shape = JoltPhysics.CapsuleShape_Create(capHalfLength, capRadius);
					mesh = GetCapsuleMesh(capRadius, capHalfLength);
					local = Matrix4x4.Identity;
					break;

				case Kind.Box:
				default:
					var half = ToJolt(geom.Size);
					// convexRadius must not exceed the smallest half extent.
					shape = JoltPhysics.BoxShape_Create(
						new Vec3 { X = half.X, Y = half.Y, Z = half.Z }, 0f);
					mesh = GetCubeMesh();
					local = Matrix4x4.CreateScale(half.X * 2, half.Y * 2, half.Z * 2);
					break;
			}

			shapes.Add(shape);

			var position = ToJolt(geom.Pos);
			var euler = ToJolt(geom.EulerDeg);
			var rotation = Quaternion.CreateFromYawPitchRoll(
				Evergine.Mathematics.MathHelper.ToRadians(euler.Y),
				Evergine.Mathematics.MathHelper.ToRadians(euler.X),
				Evergine.Mathematics.MathHelper.ToRadians(euler.Z));

			BodyCreationSettings settings = default;
			JoltPhysics.BodyCreationSettings_SetDefault(&settings);
			settings.Shape = shape;
			settings.MotionType = MotionType.Dynamic;
			settings.ObjectLayer = LayerMoving;
			settings.Position = new RVec3 { X = position.X, Y = position.Y, Z = position.Z };
			settings.Rotation = new Quat { X = rotation.X, Y = rotation.Y, Z = rotation.Z, W = rotation.W };

			uint bodyId = JoltPhysics.BodyInterface_CreateBody(bodyInterface, &settings);

			renderables.Add(new Renderable
			{
				BodyId = bodyId,
				Added = false,
				Wave = geom.Wave,
				Mesh = mesh,
				Color = new Vector4(geom.Rgb.X, geom.Rgb.Y, geom.Rgb.Z, 1f),
				Local = local,
				InitialPosition = settings.Position,
				InitialRotation = settings.Rotation,
			});
		}

		/// <summary>
		/// Adds each wave to the live world when its time comes. This is where the two engines
		/// genuinely differ: MuJoCo's model is fixed once compiled, so its demo parks the later
		/// waves off camera and pins their state every step.
		/// </summary>
		private static void ReleaseDueWaves(float time)
		{
			foreach (var renderable in renderables)
			{
				if (renderable.Added || renderable.Wave < 0 || time < WaveReleaseTime[renderable.Wave])
				{
					continue;
				}

				JoltPhysics.BodyInterface_AddBody(bodyInterface, renderable.BodyId, Activation.Activate);
				renderable.Added = true;
			}
		}

		/// <summary>
		/// Takes every dynamic body out of the world and back to its spawn pose so the drop can be
		/// watched again, which is the one thing a window has over a set of captures.
		/// </summary>
		private static void ResetScene()
		{
			foreach (var renderable in renderables)
			{
				if (renderable.Wave < 0)
				{
					continue;
				}

				if (renderable.Added)
				{
					JoltPhysics.BodyInterface_RemoveBody(bodyInterface, renderable.BodyId);
					renderable.Added = false;
				}

				JoltPhysics.BodyInterface_SetPositionAndRotation(
					bodyInterface,
					renderable.BodyId,
					renderable.InitialPosition,
					renderable.InitialRotation,
					Activation.DontActivate);
				JoltPhysics.BodyInterface_SetLinearVelocity(bodyInterface, renderable.BodyId, default);
				JoltPhysics.BodyInterface_SetAngularVelocity(bodyInterface, renderable.BodyId, default);
			}

			simulationTime = 0;
			accumulator = 0;
		}

		private static void FillConstants(Matrix4x4 viewProj)
		{
			fixed (byte* basePtr = cbData)
			{
				for (int i = 0; i < renderables.Count; i++)
				{
					var renderable = renderables[i];
					var world = renderable.Local;

					if (renderable.Added)
					{
						RVec3 position;
						Quat rotation;
						JoltPhysics.BodyInterface_GetPositionAndRotation(
							bodyInterface, renderable.BodyId, &position, &rotation);

						world = renderable.Local
							* Matrix4x4.CreateFromQuaternion(new Quaternion(rotation.X, rotation.Y, rotation.Z, rotation.W))
							* Matrix4x4.CreateTranslation(position.X, position.Y, position.Z);
					}
					else
					{
						// Not in the world yet: park it far below the ground so it is not drawn
						// hanging in mid air before its wave is released.
						world = renderable.Local * Matrix4x4.CreateTranslation(0, -1000f, 0);
					}

					var slot = (PerObject*)(basePtr + (i * CbSlotSize));
					slot->WorldViewProj = Matrix4x4.Multiply(world, viewProj);
					slot->World = world;
					slot->Color = renderable.Color;
				}
			}
		}

		private static Mesh GetCubeMesh() => GetOrCreateMesh("cube", () =>
		{
			Primitives.Cube(1f, out var vertices, out var indices);
			return (vertices, indices);
		});

		private static Mesh GetSphereMesh() => GetOrCreateMesh("sphere", () =>
		{
			Primitives.Sphere(1f, 24, out var vertices, out var indices);
			return (vertices, indices);
		});

		private static Mesh GetCapsuleMesh(float radius, float halfLength) =>
			GetOrCreateMesh($"capsule_{radius}_{halfLength}", () =>
			{
				// Capsules cannot be non-uniformly scaled without distorting the caps, so each
				// (radius, half length) pair gets its own exact mesh.
				Primitives.Capsule((halfLength + radius) * 2, radius, 16, out var vertices, out var indices);
				return (vertices, indices);
			});

		private static Mesh GetOrCreateMesh(
			string key,
			Func<(List<VertexPositionNormalTangentTexture> Vertices, List<ushort> Indices)> build)
		{
			if (meshes.TryGetValue(key, out var cached))
			{
				return cached;
			}

			var (vertexList, indexList) = build();
			var vertexArray = vertexList.ToArray();
			var indexArray = indexList.ToArray();

			var vbDescription = new BufferDescription(
				(uint)(Marshal.SizeOf<VertexPositionNormalTangentTexture>() * vertexArray.Length),
				BufferFlags.VertexBuffer, ResourceUsage.Immutable);
			var ibDescription = new BufferDescription(
				sizeof(ushort) * (uint)indexArray.Length,
				BufferFlags.IndexBuffer, ResourceUsage.Immutable);

			var mesh = new Mesh()
			{
				VertexBuffer = graphics.Factory.CreateBuffer(vertexArray, ref vbDescription),
				IndexBuffer = graphics.Factory.CreateBuffer(indexArray, ref ibDescription),
				IndexCount = (uint)indexArray.Length,
			};

			meshes[key] = mesh;
			return mesh;
		}

		private static void DrawFrame()
		{
			var commandBuffer = commandQueue.CommandBuffer();
			commandBuffer.Begin();

			fixed (byte* cbPtr = cbData)
			{
				commandBuffer.UpdateBufferData(constantBuffer, (IntPtr)cbPtr, (uint)cbData.Length);
			}

			commandBuffer.Barrier(new Buffer.Barrier(constantBuffer, Buffer.StateFlags.UniformBuffer));

			commandBuffer.SetViewports(viewports);
			commandBuffer.SetScissorRectangles(scissors);

			var renderPassDescription = new RenderPassDescription(
				frameBuffer, new ClearValue(ClearFlags.All, new Color(158, 190, 222)));
			commandBuffer.BeginRenderPass(ref renderPassDescription);
			commandBuffer.SetGraphicsPipelineState(pipeline);

			var offsets = new uint[1];
			for (int i = 0; i < renderables.Count; i++)
			{
				var mesh = renderables[i].Mesh;
				offsets[0] = (uint)i * CbSlotSize;
				commandBuffer.SetResourceSet(resourceSet, 0, offsets);
				commandBuffer.SetVertexBuffers(new[] { mesh.VertexBuffer });
				commandBuffer.SetIndexBuffer(mesh.IndexBuffer);
				commandBuffer.DrawIndexed(mesh.IndexCount);
			}

			commandBuffer.EndRenderPass();
			commandBuffer.End();
			commandBuffer.Commit();

			commandQueue.Submit();
			commandQueue.WaitIdle();
		}

		private static void Shutdown()
		{
			// Teardown, in the order JoltTestEnvironment.Dispose uses.
			foreach (var renderable in renderables)
			{
				if (renderable.Added)
				{
					JoltPhysics.BodyInterface_RemoveBody(bodyInterface, renderable.BodyId);
				}

				JoltPhysics.BodyInterface_DestroyBody(bodyInterface, renderable.BodyId);
			}

			foreach (var shape in shapes)
			{
				JoltPhysics.Shape_Destroy(shape);
			}

			JoltPhysics.PhysicsSystem_Destroy(physicsSystem);
			JoltPhysics.ObjectLayerPairFilter_Destroy(objectLayerPairFilter);
			JoltPhysics.ObjectVsBroadPhaseLayerFilter_Destroy(objectVsBroadPhaseFilter);
			JoltPhysics.BroadPhaseLayerInterface_Destroy(broadPhase);
			JoltPhysics.JobSystem_Destroy(jobSystem);
			JoltPhysics.TempAllocator_Destroy(tempAllocator);
			JoltPhysics.UnregisterTypes();
			JoltPhysics.DestroyFactory();
			JoltPhysics.Shutdown();

			swapChain.Dispose();
			graphics.Dispose();
		}
	}
}
