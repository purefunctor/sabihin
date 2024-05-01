open Js.Typed_array;
open WebCrypto;

let computeBase = (salt: Uint8Array.t) => {
  assert(Uint8Array.length(salt) == 16);
  let textEncoder = TextEncoder.create();
  let saltBuffer = Uint8Array.make(Array.make(256, 0));

  // Encode first 10 bytes.
  TextEncoder.encodeInto(textEncoder, "sabihin.ph", saltBuffer);

  // Encode next 230 bytes.
  for (index in 10 to 240) {
    Uint8Array.unsafe_set(saltBuffer, index, Char.code('L'));
  };

  // Encode next 16 bytes.
  for (index in 0 to 16) {
    Uint8Array.unsafe_set(
      saltBuffer,
      index + 240,
      Uint8Array.unsafe_get(salt, index),
    );
  };

  saltBuffer;
};

let computeDigest = (salt: Uint8Array.t) => {
  digest_impl("SHA-256", computeBase(salt));
};

// From: https://developer.mozilla.org/en-US/docs/Web/API/SubtleCrypto/digest
let toHash: ArrayBuffer.t => string = [%mel.raw
  {|(data) => {
      const data_array = Array.from(new Uint8Array(data));
      return data_array.map((c) => c.toString(16).padStart(2, "0")).join("");
    }|}
];
