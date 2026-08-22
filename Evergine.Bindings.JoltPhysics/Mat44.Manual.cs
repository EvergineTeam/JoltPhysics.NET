namespace Evergine.Bindings.JoltPhysics
{
	/// <summary>
	/// Hand-written companion to the generated <see cref="Mat44"/>.
	/// The generator emits the sixteen elements as explicit fields (M_0..M_15) rather than a
	/// C# fixed buffer, because the wasm P/Invoke table generator collapses a struct whose only
	/// member is a primitive fixed buffer to a single scalar, breaking every entry point that
	/// takes a Mat44 by value under the interpreter. This partial gives indexed access back.
	/// </summary>
	public unsafe partial struct Mat44
	{
		/// <summary>
		/// Gets or sets the matrix element at <paramref name="index"/> (0..15), in the same
		/// column-major order the old fixed buffer exposed.
		/// </summary>
		public float this[int index]
		{
			get
			{
				fixed (float* m = &this.M_0)
				{
					return m[index];
				}
			}

			set
			{
				fixed (float* m = &this.M_0)
				{
					m[index] = value;
				}
			}
		}
	}
}
