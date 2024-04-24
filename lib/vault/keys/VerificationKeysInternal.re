open Js.Typed_array;
open PromiseUtil;
open WebCrypto;

type privateKey = cryptoKey;
type publicKey = cryptoKey;

type freshKeys = {
  privateKey,
  publicKey,
};

let create = () => {
  let* keyPair = {
    let algorithm = {
      "name": "RSA-PSS",
      "modulusLength": 4096,
      "publicExponent": Uint8Array.make([|1, 0, 1|]),
      "hash": "SHA-512",
    };
    let extractable = true;
    let keyUsages = [|"sign", "verify"|];
    generateKeyPair_impl(algorithm, extractable, keyUsages);
  };
  resolve({privateKey: keyPair.privateKey, publicKey: keyPair.publicKey});
};

external privateFromCryptoKey: cryptoKey => privateKey = "%identity";
external privateToCryptoKey: privateKey => cryptoKey = "%identity";
external publicFromCryptoKey: cryptoKey => publicKey = "%identity";
external publicToCryptoKey: publicKey => cryptoKey = "%identity";

module type S = {
  type nonrec privateKey = privateKey;
  type nonrec publicKey = publicKey;
  type nonrec freshKeys = freshKeys;

  let create: unit => Js.Promise.t(freshKeys);
};
