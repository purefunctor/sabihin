open Js.Typed_array;

type t;

[@mel.new] external create: unit => t = "TextDecoder";

[@mel.send] external decode: (t, Uint8Array.t) => string = "decode";
