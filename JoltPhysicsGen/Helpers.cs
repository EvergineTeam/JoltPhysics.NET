using CppAst;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace JoltPhysicsGen
{
	public static class Helpers
	{
		/// <summary>
		/// List of opaque handle typedef names (resolve to IntPtr).
		/// </summary>
		public static List<string> OpaqueHandles = new();

		/// <summary>
		/// List of function-pointer typedef names (resolve to IntPtr or delegates).
		/// </summary>
		public static List<string> DelegateNames = new();

		/// <summary>
		/// Mapping of typedef names to C# primitive types.
		/// </summary>
		private static readonly Dictionary<string, string> csNameMappings = new()
		{
			{ "bool", "bool" },
			{ "uint8_t", "byte" },
			{ "uint16_t", "ushort" },
			{ "uint32_t", "uint" },
			{ "uint64_t", "ulong" },
			{ "int8_t", "sbyte" },
			{ "int16_t", "short" },
			{ "int32_t", "int" },
			{ "int64_t", "long" },
			{ "char", "byte" },
			{ "size_t", "nuint" },
			{ "intptr_t", "nint" },
			{ "uintptr_t", "nuint" },
			// JoltC value-type aliases
			{ "JoltC_Bool", "int" },
			{ "JoltC_BodyID", "uint" },
			{ "JoltC_ObjectLayer", "ushort" },
			{ "JoltC_BroadPhaseLayer", "byte" },
			{ "JoltC_SubShapeID", "uint" },
			{ "JoltC_CollisionGroupID", "uint" },
			{ "JoltC_CollisionSubGroupID", "uint" },
		};

		/// <summary>
		/// Known blittable struct names (not opaque handles).
		/// </summary>
		public static HashSet<string> BlittableStructs = new();

		/// <summary>
		/// Convert a CppAst type to its C# equivalent string.
		/// </summary>
		public static string ConvertToCSharpType(CppType type, bool isPointer = false)
		{
			if (type is CppPrimitiveType primitiveType)
			{
				return primitiveType.Kind switch
				{
					CppPrimitiveKind.Void => isPointer ? "void" : "void",
					CppPrimitiveKind.Bool => "bool",
					CppPrimitiveKind.WChar => "char",
					CppPrimitiveKind.Char => "byte",
					CppPrimitiveKind.Short => "short",
					CppPrimitiveKind.Int => "int",
					CppPrimitiveKind.LongLong => "long",
					CppPrimitiveKind.UnsignedChar => "byte",
					CppPrimitiveKind.UnsignedShort => "ushort",
					CppPrimitiveKind.UnsignedInt => "uint",
					CppPrimitiveKind.UnsignedLongLong => "ulong",
					CppPrimitiveKind.Float => "float",
					CppPrimitiveKind.Double => "double",
					CppPrimitiveKind.LongDouble => "double",
					_ => "int"
				};
			}

			if (type is CppQualifiedType qualifiedType)
			{
				return ConvertToCSharpType(qualifiedType.ElementType, isPointer);
			}

			if (type is CppEnum enumType)
			{
				return StripPrefix(enumType.Name);
			}

			if (type is CppTypedef typedefType)
			{
				return GetCsCleanName(typedefType.Name);
			}

			if (type is CppClass classType)
			{
				string name = classType.Name;
				if (BlittableStructs.Contains(name))
				{
					return StripPrefix(name);
				}

				if (OpaqueHandles.Contains(name))
				{
					return isPointer ? "IntPtr" : "IntPtr";
				}

				// Check name mappings
				if (csNameMappings.TryGetValue(name, out var mapped))
				{
					return mapped;
				}

				return StripPrefix(name);
			}

			if (type is CppPointerType pointerType)
			{
				var elementType = pointerType.ElementType;

				// void* → void*
				if (elementType is CppPrimitiveType prim && prim.Kind == CppPrimitiveKind.Void)
				{
					return "void*";
				}

				// const char* → byte*
				if (IsConstCharPointer(pointerType))
				{
					return "byte*";
				}

				// Function pointer → IntPtr (or delegate name)
				if (elementType is CppFunctionType)
				{
					return "IntPtr";
				}

				var innerCsType = ConvertToCSharpType(elementType, true);

				// Opaque handles → IntPtr (already an IntPtr, no need for pointer)
				if (OpaqueHandles.Contains(GetUnqualifiedName(elementType)))
				{
					return "IntPtr";
				}

				// Pointer to pointer of opaque handle → IntPtr*
				if (elementType is CppPointerType innerPtr)
				{
					var deepName = GetUnqualifiedName(innerPtr.ElementType);
					if (OpaqueHandles.Contains(deepName))
					{
						return "IntPtr*";
					}
				}

				return innerCsType + "*";
			}

			if (type is CppArrayType arrayType)
			{
				return ConvertToCSharpType(arrayType.ElementType);
			}

			return type.ToString();
		}

		/// <summary>
		/// Get the unqualified name of a type (strip const/volatile).
		/// </summary>
		public static string GetUnqualifiedName(CppType type)
		{
			if (type is CppQualifiedType q)
				return GetUnqualifiedName(q.ElementType);
			if (type is CppTypedef td)
				return td.Name;
			if (type is CppClass cls)
				return cls.Name;
			if (type is CppEnum en)
				return en.Name;
			return type.ToString();
		}

		/// <summary>
		/// Check if a pointer type is const char*.
		/// </summary>
		public static bool IsConstCharPointer(CppPointerType pointerType)
		{
			var inner = pointerType.ElementType;
			if (inner is CppQualifiedType qual)
				inner = qual.ElementType;
			if (inner is CppPrimitiveType prim && prim.Kind == CppPrimitiveKind.Char)
				return true;
			return false;
		}

		/// <summary>
		/// Resolve a typedef name to its C# equivalent.
		/// </summary>
		public static string GetCsCleanName(string name)
		{
			if (csNameMappings.TryGetValue(name, out var mapped))
				return mapped;

			if (OpaqueHandles.Contains(name))
				return "IntPtr";

			if (DelegateNames.Contains(name))
				return StripPrefix(name);

			return StripPrefix(name);
		}

		/// <summary>
		/// Convert a C type for use as a function parameter.
		/// </summary>
		public static string ConvertParameterType(CppType type)
		{
			return ConvertToCSharpType(type, false);
		}

		/// <summary>
		/// Convert a C type for use as a function return type.
		/// </summary>
		public static string ConvertReturnType(CppType type)
		{
			return ConvertToCSharpType(type, false);
		}

		/// <summary>
		/// Convert a C type for use as a struct field.
		/// </summary>
		public static string ConvertFieldType(CppType type, out int fixedArraySize)
		{
			fixedArraySize = 0;

			if (type is CppArrayType arrayType)
			{
				fixedArraySize = arrayType.Size;
				return ConvertToCSharpType(arrayType.ElementType, false);
			}

			if (type is CppPointerType pointerType)
			{
				// Function pointer fields → IntPtr
				var inner = pointerType.ElementType;
				if (inner is CppQualifiedType q)
					inner = q.ElementType;
				if (inner is CppFunctionType)
					return "IntPtr";

				return ConvertToCSharpType(type, false);
			}

			return ConvertToCSharpType(type, false);
		}

		/// <summary>
		/// Capitalize the first letter of a field name (camelCase → PascalCase).
		/// Preserves existing uppercase runs like "ID" in "bodyID" → "BodyID".
		/// </summary>
		public static string PascalCaseField(string name)
		{
			if (string.IsNullOrEmpty(name)) return name;
			return char.ToUpper(name[0]) + name.Substring(1);
		}

		/// <summary>
		/// Strip the JoltC_ or JOLTC_ prefix from a C name.
		/// </summary>
		public static string StripPrefix(string name)
		{
			if (name.StartsWith("JoltC_"))
				return name.Substring(6);
			if (name.StartsWith("JOLTC_"))
				return name.Substring(6);
			return name;
		}

		/// <summary>
		/// Find the longest common prefix (at underscore boundary) among a list of SCREAMING_CASE names.
		/// </summary>
		public static string FindCommonPrefix(IEnumerable<string> names)
		{
			var list = names.ToList();
			if (list.Count == 0) return "";
			if (list.Count == 1)
			{
				int lastUnderscore = list[0].LastIndexOf('_');
				return lastUnderscore >= 0 ? list[0].Substring(0, lastUnderscore + 1) : "";
			}

			string first = list[0];
			int prefixLen = first.Length;

			for (int i = 1; i < list.Count; i++)
			{
				string current = list[i];
				int maxLen = Math.Min(prefixLen, current.Length);
				int j = 0;
				while (j < maxLen && first[j] == current[j])
					j++;
				prefixLen = j;
			}

			string rawPrefix = first.Substring(0, prefixLen);
			int lastUs = rawPrefix.LastIndexOf('_');
			return lastUs >= 0 ? rawPrefix.Substring(0, lastUs + 1) : "";
		}

		/// <summary>
		/// Convert a SCREAMING_CASE name to PascalCase (e.g. DONT_ACTIVATE → DontActivate).
		/// </summary>
		public static string ScreamingToPascalCase(string screaming)
		{
			if (string.IsNullOrEmpty(screaming)) return screaming;

			var parts = screaming.Split('_', StringSplitOptions.RemoveEmptyEntries);
			var sb = new StringBuilder();
			foreach (var part in parts)
			{
				if (part.Length == 0) continue;
				if (char.IsDigit(part[0]))
				{
					sb.Append(part); // Keep numeric-leading parts as-is (e.g. "2D")
				}
				else
				{
					sb.Append(char.ToUpper(part[0]));
					sb.Append(part.Substring(1).ToLower());
				}
			}
			return sb.ToString();
		}

		/// <summary>
		/// Escape C# reserved keywords by prefixing with @.
		/// </summary>
		public static string EscapeReservedKeyword(string name)
		{
			return name switch
			{
				"abstract" or "as" or "base" or "bool" or "break" or "byte" or "case" or
				"catch" or "char" or "checked" or "class" or "const" or "continue" or
				"decimal" or "default" or "delegate" or "do" or "double" or "else" or
				"enum" or "event" or "explicit" or "extern" or "false" or "finally" or
				"fixed" or "float" or "for" or "foreach" or "goto" or "if" or "implicit" or
				"in" or "int" or "interface" or "internal" or "is" or "lock" or "long" or
				"namespace" or "new" or "null" or "object" or "operator" or "out" or
				"override" or "params" or "private" or "protected" or "public" or
				"readonly" or "ref" or "return" or "sbyte" or "sealed" or "short" or
				"sizeof" or "stackalloc" or "static" or "string" or "struct" or "switch" or
				"this" or "throw" or "true" or "try" or "typeof" or "uint" or "ulong" or
				"unchecked" or "unsafe" or "ushort" or "using" or "virtual" or "void" or
				"volatile" or "while" => "@" + name,
				_ => name
			};
		}

		/// <summary>
		/// Write XML documentation comment from a CppComment.
		/// </summary>
		public static void WriteComment(System.IO.StreamWriter writer, CppComment comment, string tabs)
		{
			if (comment == null) return;

			var text = comment.ToString().Trim();
			if (string.IsNullOrEmpty(text)) return;

			var lines = text.Split('\n');
			writer.WriteLine($"{tabs}/// <summary>");
			foreach (var line in lines)
			{
				var cleaned = line.Trim().TrimStart('/', '*', ' ');
				if (!string.IsNullOrWhiteSpace(cleaned))
				{
					writer.WriteLine($"{tabs}/// {System.Security.SecurityElement.Escape(cleaned)}");
				}
			}
			writer.WriteLine($"{tabs}/// </summary>");
		}
	}
}
