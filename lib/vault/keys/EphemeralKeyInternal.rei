open WebCrypto;

type t;
type messageIv;

type freshKey = {
  ephemeralKey: t,
  messageIv,
};

let create: unit => Js.Promise.t(freshKey);

external fromCryptoKey: cryptoKey => t = "%identity";
external toCryptoKey: t => cryptoKey = "%identity";

module type S = {
  type nonrec t = t;
  type nonrec messageIv = messageIv;
  type nonrec freshKey = freshKey;

  let create: unit => Js.Promise.t(freshKey);
};
