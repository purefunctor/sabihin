open Js.Typed_array;

module TextEncoder = {
  type t;

  [@mel.new] external create: unit => t = "TextEncoder";

  [@mel.send] external encode: (t, string) => Uint8Array.t = "encode";

  [@mel.send]
  external encodeInto: (t, string, Uint8Array.t) => unit = "encodeInto";
};

module TextDecoder = {
  type t;

  [@mel.new] external create: unit => t = "TextDecoder";

  [@mel.send] external decode: (t, Uint8Array.t) => string = "decode";
};

module WebCryptoAPI = {
  type crypto_key = {
    [@mel.as "type"]
    type_: string,
    extractable: bool,
    algorithm: string,
    usages: array(string),
  };

  type crypto_key_pair = {
    [@mel.as "privateKey"]
    private_key: crypto_key,
    [@mel.as "publicKey"]
    public_key: crypto_key,
  };

  [@mel.scope "window.crypto.subtle"]
  external decrypt_impl:
    (Js.t({..}), crypto_key, ArrayBuffer.t) => Js.Promise.t(ArrayBuffer.t) =
    "decrypt";

  [@mel.scope "window.crypto.subtle"]
  external deriveBits_impl:
    (Js.t({..}), crypto_key, int) => Js.Promise.t(ArrayBuffer.t) =
    "deriveBits";

  [@mel.scope "window.crypto.subtle"]
  external deriveKey_impl:
    (Js.t({..}), crypto_key, Js.t({..}), bool, array(string)) =>
    Js.Promise.t(crypto_key) =
    "deriveKey";

  [@mel.scope "window.crypto.subtle"]
  external digest_impl: (string, Uint8Array.t) => Js.Promise.t(ArrayBuffer.t) =
    "digest";

  [@mel.scope "window.crypto.subtle"]
  external encrypt_impl:
    (Js.t({..}), crypto_key, ArrayBuffer.t) => Js.Promise.t(ArrayBuffer.t) =
    "encrypt";

  [@mel.scope "window.crypto.subtle"]
  external generateKey_impl:
    (Js.t({..}), bool, array(string)) => Js.Promise.t(crypto_key) =
    "generateKey";

  [@mel.scope "window.crypto.subtle"]
  external generateKeyPair_impl:
    (Js.t({..}), bool, array(string)) => Js.Promise.t(crypto_key_pair) =
    "generateKey";

  [@mel.scope "window.crypto"]
  external getRandomValues_impl: Uint8Array.t => Uint8Array.t =
    "getRandomValues";

  [@mel.scope "window.crypto.subtle"]
  external importKey_impl:
    (string, Uint8Array.t, 'a, bool, array(string)) =>
    Js.Promise.t(crypto_key) =
    "importKey";

  [@mel.scope "window.crypto.subtle"]
  external unwrapKey_impl:
    (
      string,
      ArrayBuffer.t,
      crypto_key,
      Js.t({..}),
      Js.t({..}),
      bool,
      array(string)
    ) =>
    Js.Promise.t(crypto_key) =
    "unwrapKey";

  [@mel.scope "window.crypto.subtle"]
  external wrapKey_impl:
    (string, crypto_key, crypto_key, Js.t({..})) =>
    Js.Promise.t(ArrayBuffer.t) =
    "wrapKey";
};

module Salt = {
  open WebCryptoAPI;

  let compute = (salt: Uint8Array.t) => {
    let salt_length = Uint8Array.length(salt);
    let text_encoder = TextEncoder.create();
    let salt_buffer = Uint8Array.make(Array.make(256 + salt_length, 0));

    // Encode first 10 bytes.
    TextEncoder.encodeInto(text_encoder, "sabihin.ph", salt_buffer);

    // Encode next 246 bytes.
    for (index in 10 to 256) {
      Uint8Array.unsafe_set(salt_buffer, index, Char.code('L'));
    };

    // Encode next `salt_length` bytes.
    for (index in 0 to salt_length) {
      Uint8Array.unsafe_set(
        salt_buffer,
        index + 256,
        Uint8Array.unsafe_get(salt, index),
      );
    };

    digest_impl("SHA-256", salt_buffer);
  };

  // From: https://developer.mozilla.org/en-US/docs/Web/API/SubtleCrypto/digest
  let toHash: ArrayBuffer.t => string = [%mel.raw
    {|
    function toHash(data) {
      const data_array = Array.from(new Uint8Array(data));
      return data_array.map((c) => c.toString(16).padStart(2, "0")).join("");
    }
  |}
  ];
};

module MasterKey = {
  open WebCryptoAPI;

  type fresh_t = {
    master_key: crypto_key,
    client_random: Uint8Array.t,
    salt_buffer: ArrayBuffer.t,
  };

  let create = () => {
    let random_data = getRandomValues_impl(Uint8Array.fromLength(128));
    let client_random = getRandomValues_impl(Uint8Array.fromLength(128));

    let import_random_as_base_key = {
      let format = "raw";
      let keyData = random_data;
      let algorithm = "HKDF";
      let extractable = false;
      let keyUsages = [|"deriveKey"|];
      importKey_impl(format, keyData, algorithm, extractable, keyUsages);
    };

    let compute_salt = Salt.compute(client_random);

    let derive_master_key = ((base_key, salt_buffer)) => {
      let algoritm = {
        "name": "HKDF",
        "hash": "SHA-512",
        "salt": salt_buffer,
        "info": Uint8Array.make([||]),
      };
      let baseKey = base_key;
      let derivedKeyAlgorithm = {"name": "AES-GCM", "length": 256};
      let extractable = true;
      let keyUsages = [|"wrapKey", "unwrapKey"|];
      Js.Promise.(
        deriveKey_impl(
          algoritm,
          baseKey,
          derivedKeyAlgorithm,
          extractable,
          keyUsages,
        )
        |> then_(master_key => {
             resolve({master_key, client_random, salt_buffer})
           })
      );
    };

    Js.Promise.(
      all2((import_random_as_base_key, compute_salt))
      |> then_(derive_master_key)
    );
  };
};

module DerivedKey = {
  open WebCryptoAPI;

  type fresh_t = {
    derived_encryption_key: crypto_key,
    hashed_authentication_key: ArrayBuffer.t,
    encryption_iv: Uint8Array.t,
  };

  let create = (password: string, salt_buffer: ArrayBuffer.t) => {
    let text_encoder = TextEncoder.create();
    let password_raw = TextEncoder.encode(text_encoder, password);
    let encryption_iv = getRandomValues_impl(Uint8Array.fromLength(12));

    let import_password_as_base_key = {
      let format = "raw";
      let keyData = password_raw;
      let algorithm = "PBKDF2";
      let extractable = false;
      let keyUsages = [|"deriveBits"|];
      importKey_impl(format, keyData, algorithm, extractable, keyUsages);
    };

    let derive_bits_from_base_key = base_key => {
      let algorithm = {
        "name": "PBKDF2",
        "hash": "SHA-512",
        "salt": salt_buffer,
        "iterations": 100_000,
      };
      let baseKey = base_key;
      let length = 256;
      deriveBits_impl(algorithm, baseKey, length);
    };

    let derive_encryption_key = key_bits => {
      let derived_encryption_key_raw =
        Uint8Array.fromBuffer(~off=0, ~len=16, key_bits, ());
      let format = "raw";
      let keyData = derived_encryption_key_raw;
      let algorithm = {"name": "AES-GCM"};
      let extractable = false;
      let keyUsages = [|"wrapKey", "unwrapKey"|];
      importKey_impl(format, keyData, algorithm, extractable, keyUsages);
    };

    let hash_authentication_key = key_bits => {
      let derived_authentication_key =
        Uint8Array.fromBuffer(~off=16, ~len=16, key_bits, ());
      digest_impl("SHA-256", derived_authentication_key);
    };

    let derive_encryption_hash_authentication = key_bits => {
      Js.Promise.(
        all2((
          derive_encryption_key(key_bits),
          hash_authentication_key(key_bits),
        ))
        |> then_(((derived_encryption_key, hashed_authentication_key)) => {
             resolve({
               derived_encryption_key,
               hashed_authentication_key,
               encryption_iv,
             })
           })
      );
    };

    Js.Promise.(
      import_password_as_base_key
      |> then_(derive_bits_from_base_key)
      |> then_(derive_encryption_hash_authentication)
    );
  };

  let wrap =
      (
        derive_key: crypto_key,
        encryption_iv: ArrayBuffer.t,
        master_key: crypto_key,
      ) => {
    let key = master_key;
    let wrappingKey = derive_key;
    let wrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    wrapKey_impl("raw", key, wrappingKey, wrapAlgo);
  };

  let unwrap =
      (
        derived_key: crypto_key,
        encryption_iv: ArrayBuffer.t,
        encrypted_master_key: ArrayBuffer.t,
      ) => {
    let format = "raw";
    let wrappedKey = encrypted_master_key;
    let unwrappingKey = derived_key;
    let unwrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    let unwrappedKeyAlgo = {"name": "AES-GCM"};
    let extractable = true;
    let keyUsages = [|"wrapKey", "unwrapKey"|];
    unwrapKey_impl(
      format,
      wrappedKey,
      unwrappingKey,
      unwrapAlgo,
      unwrappedKeyAlgo,
      extractable,
      keyUsages,
    );
  };
};

module RsaKeyPair = {
  open WebCryptoAPI;

  type fresh_t = crypto_key_pair;

  let create = () => {
    let algorithm = {
      "name": "RSA-OAEP",
      "modulusLength": 4096,
      "publicExponent": Uint8Array.make([|1, 0, 1|]),
      "hash": "SHA-512",
    };
    let extractable = true;
    let keyUsages = [|"wrapKey", "unwrapKey"|];
    generateKeyPair_impl(algorithm, extractable, keyUsages);
  };

  let wrap = (public_key: crypto_key, ephemeral_key: crypto_key) => {
    let format = "raw";
    let key = ephemeral_key;
    let wrappingKey = public_key;
    let wrapAlgo = {"name": "RSA-OAEP"};
    wrapKey_impl(format, key, wrappingKey, wrapAlgo);
  };

  let unwrap = (private_key: crypto_key, ephemeral_key: ArrayBuffer.t) => {
    let format = "raw";
    let wrappedKey = ephemeral_key;
    let unwrappingKey = private_key;
    let unwrapAlgo = {"name": "RSA-OAEP"};
    let unwrappedKeyAlgo = {"name": "AES-GCM"};
    let extractable = true;
    let keyUsages = [|"encrypt", "decrypt"|];
    unwrapKey_impl(
      format,
      wrappedKey,
      unwrappingKey,
      unwrapAlgo,
      unwrappedKeyAlgo,
      extractable,
      keyUsages,
    );
  };
};

module EphemeralKey = {
  open WebCryptoAPI;

  type fresh_t = {
    ephemeral_key: crypto_key,
    encryption_iv: Uint8Array.t,
  };

  let create = () => {
    let encryption_iv = getRandomValues_impl(Uint8Array.fromLength(12));
    let algorithm = {"name": "AES-GCM", "length": "256"};
    let extractable = true;
    let keyUsages = [|"encrypt", "decrypt"|];
    Js.Promise.(
      generateKey_impl(algorithm, extractable, keyUsages)
      |> then_(ephemeral_key => {resolve({ephemeral_key, encryption_iv})})
    );
  };
};
