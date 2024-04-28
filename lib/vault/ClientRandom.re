open Js.Typed_array;
open WebCrypto;

let create = () => {
  getRandomValues_impl(Uint8Array.fromLength(16));
};
