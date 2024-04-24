open WebCrypto;

type privateKey;
type publicKey;

type freshKeys = {
  privateKey,
  publicKey,
};

let create: unit => Js.Promise.t(freshKeys);

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
