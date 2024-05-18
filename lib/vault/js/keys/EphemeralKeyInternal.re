open Js.Typed_array;
open Promise_syntax;
open WebCrypto;

type t = cryptoKey;

type freshKey = {
  ephemeralKey: t,
  messageIv: Uint8Array.t,
};

let create = () => {
  let messageIv = getRandomValues_impl(Uint8Array.fromLength(12));

  let* ephemeralKey = {
    let algorithm = {"name": "AES-GCM", "length": "256"};
    let extractable = true;
    let keyUsages = [|"encrypt", "decrypt"|];
    generateKey_impl(algorithm, extractable, keyUsages);
  };

  resolve({ephemeralKey, messageIv});
};

let fromCryptoKey: cryptoKey => t = Fun.id;
let toCryptoKey: t => cryptoKey = Fun.id;
