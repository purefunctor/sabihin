open Js.Typed_array;
open WebCrypto;

type t = pri cryptoKey;

type derivedSecrets = {
  derivedKey: t,
  authenticationKey: string,
  masterKeyIv: Uint8Array.t,
  protectionKeyIv: Uint8Array.t,
  verificationKeyIv: Uint8Array.t,
};

let create:
  (~password: string, ~saltBuffer: ArrayBuffer.t) =>
  Js.Promise.t(derivedSecrets);

let fromCryptoKey: cryptoKey => t;
let toCryptoKey: t => cryptoKey;
