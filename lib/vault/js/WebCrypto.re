open Js.Typed_array;

type cryptoKey = {
  [@mel.as "type"]
  type_: string,
  extractable: bool,
  algorithm: string,
  usages: array(string),
};

type cryptoKeyPair = {
  privateKey: cryptoKey,
  publicKey: cryptoKey,
};

[@mel.scope "crypto.subtle"]
external decrypt_impl:
  (Js.t({..}), cryptoKey, ArrayBuffer.t) => Js.Promise.t(ArrayBuffer.t) =
  "decrypt";

[@mel.scope "crypto.subtle"]
external deriveBits_impl:
  (Js.t({..}), cryptoKey, int) => Js.Promise.t(ArrayBuffer.t) =
  "deriveBits";

[@mel.scope "crypto.subtle"]
external deriveKey_impl:
  (Js.t({..}), cryptoKey, Js.t({..}), bool, array(string)) =>
  Js.Promise.t(cryptoKey) =
  "deriveKey";

[@mel.scope "crypto.subtle"]
external digest_impl: (string, Uint8Array.t) => Js.Promise.t(ArrayBuffer.t) =
  "digest";

[@mel.scope "crypto.subtle"]
external encrypt_impl:
  (Js.t({..}), cryptoKey, ArrayBuffer.t) => Js.Promise.t(ArrayBuffer.t) =
  "encrypt";

[@mel.scope "crypto.subtle"]
external generateKey_impl:
  (Js.t({..}), bool, array(string)) => Js.Promise.t(cryptoKey) =
  "generateKey";

[@mel.scope "crypto.subtle"]
external generateKeyPair_impl:
  (Js.t({..}), bool, array(string)) => Js.Promise.t(cryptoKeyPair) =
  "generateKey";

[@mel.scope "crypto"]
external getRandomValues_impl: Uint8Array.t => Uint8Array.t =
  "getRandomValues";

[@mel.scope "crypto.subtle"]
external exportKey_impl: (string, cryptoKey) => Js.Promise.t(ArrayBuffer.t) =
  "exportKey";

[@mel.scope "crypto.subtle"]
external importKey_impl:
  (string, ArrayBuffer.t, 'a, bool, array(string)) => Js.Promise.t(cryptoKey) =
  "importKey";

[@mel.scope "crypto.subtle"]
external unwrapKey_impl:
  (
    string,
    ArrayBuffer.t,
    cryptoKey,
    Js.t({..}),
    Js.t({..}),
    bool,
    array(string)
  ) =>
  Js.Promise.t(cryptoKey) =
  "unwrapKey";

[@mel.scope "crypto.subtle"]
external wrapKey_impl:
  (string, cryptoKey, cryptoKey, Js.t({..})) => Js.Promise.t(ArrayBuffer.t) =
  "wrapKey";

[@mel.scope "crypto.subtle"]
external sign_impl:
  (Js.t({..}), cryptoKey, ArrayBuffer.t) => Js.Promise.t(ArrayBuffer.t) =
  "sign";

[@mel.scope "crypto.subtle"]
external verify_impl:
  (Js.t({..}), cryptoKey, ArrayBuffer.t, ArrayBuffer.t) => Js.Promise.t(bool) =
  "verify";
