open Js.Typed_array;

type t;

[@mel.new] external create: unit => t = "TextEncoder";

[@mel.send] external encode: (t, string) => Uint8Array.t = "encode";

[@mel.send]
external encodeInto: (t, string, Uint8Array.t) => unit = "encodeInto";
