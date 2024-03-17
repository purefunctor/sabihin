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

  [@mel.scope "crypto.subtle"]
  external decrypt_impl:
    (Js.t({..}), crypto_key, ArrayBuffer.t) => Js.Promise.t(ArrayBuffer.t) =
    "decrypt";

  [@mel.scope "crypto.subtle"]
  external deriveBits_impl:
    (Js.t({..}), crypto_key, int) => Js.Promise.t(ArrayBuffer.t) =
    "deriveBits";

  [@mel.scope "crypto.subtle"]
  external deriveKey_impl:
    (Js.t({..}), crypto_key, Js.t({..}), bool, array(string)) =>
    Js.Promise.t(crypto_key) =
    "deriveKey";

  [@mel.scope "crypto.subtle"]
  external digest_impl: (string, Uint8Array.t) => Js.Promise.t(ArrayBuffer.t) =
    "digest";

  [@mel.scope "crypto.subtle"]
  external encrypt_impl:
    (Js.t({..}), crypto_key, ArrayBuffer.t) => Js.Promise.t(ArrayBuffer.t) =
    "encrypt";

  [@mel.scope "crypto.subtle"]
  external generateKey_impl:
    (Js.t({..}), bool, array(string)) => Js.Promise.t(crypto_key) =
    "generateKey";

  [@mel.scope "crypto.subtle"]
  external generateKeyPair_impl:
    (Js.t({..}), bool, array(string)) => Js.Promise.t(crypto_key_pair) =
    "generateKey";

  [@mel.scope "crypto"]
  external getRandomValues_impl: Uint8Array.t => Uint8Array.t =
    "getRandomValues";

  [@mel.scope "crypto.subtle"]
  external importKey_impl:
    (string, Uint8Array.t, 'a, bool, array(string)) =>
    Js.Promise.t(crypto_key) =
    "importKey";

  [@mel.scope "crypto.subtle"]
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

  [@mel.scope "crypto.subtle"]
  external wrapKey_impl:
    (string, crypto_key, crypto_key, Js.t({..})) =>
    Js.Promise.t(ArrayBuffer.t) =
    "wrapKey";

  [@mel.scope "crypto.subtle"]
  external sign_impl:
    (Js.t({..}), crypto_key, ArrayBuffer.t) => Js.Promise.t(ArrayBuffer.t) =
    "sign";

  [@mel.scope "crypto.subtle"]
  external verify_impl:
    (Js.t({..}), crypto_key, ArrayBuffer.t, ArrayBuffer.t) =>
    Js.Promise.t(bool) =
    "verify";
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

  type key_t =
    | MasterKey(crypto_key);

  type protection_iv_t =
    | ProtectionIv(Uint8Array.t);
  type verification_iv_t =
    | VerificationIv(Uint8Array.t);

  type fresh_t = {
    master_key: key_t,
    client_random: Uint8Array.t,
    salt_buffer: ArrayBuffer.t,
    protection_private_encryption_iv: protection_iv_t,
    verification_private_encryption_iv: verification_iv_t,
  };

  let create = () => {
    let random_data = getRandomValues_impl(Uint8Array.fromLength(128));
    let client_random = getRandomValues_impl(Uint8Array.fromLength(128));
    let protection_private_encryption_iv =
      ProtectionIv(getRandomValues_impl(Uint8Array.fromLength(12)));
    let verification_private_encryption_iv =
      VerificationIv(getRandomValues_impl(Uint8Array.fromLength(12)));

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
             let master_key = MasterKey(master_key);
             resolve({
               master_key,
               client_random,
               salt_buffer,
               protection_private_encryption_iv,
               verification_private_encryption_iv,
             });
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

  type key_t =
    | DerivedKey(crypto_key);

  type master_iv_t =
    | MasterIv(Uint8Array.t);

  type fresh_t = {
    derived_encryption_key: key_t,
    hashed_authentication_key: ArrayBuffer.t,
    master_encryption_iv: master_iv_t,
  };

  let create = (password: string, salt_buffer: ArrayBuffer.t) => {
    let text_encoder = TextEncoder.create();
    let password_raw = TextEncoder.encode(text_encoder, password);
    let master_encryption_iv =
      MasterIv(getRandomValues_impl(Uint8Array.fromLength(12)));

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
             let derived_encryption_key = DerivedKey(derived_encryption_key);
             resolve({
               derived_encryption_key,
               hashed_authentication_key,
               master_encryption_iv,
             });
           })
      );
    };

    Js.Promise.(
      import_password_as_base_key
      |> then_(derive_bits_from_base_key)
      |> then_(derive_encryption_hash_authentication)
    );
  };
};

module ProtectionKeyPair = {
  open WebCryptoAPI;

  type public_key_t =
    | PublicKey(crypto_key);
  type private_key_t =
    | PrivateKey(crypto_key);

  type fresh_t = {
    private_key: private_key_t,
    public_key: public_key_t,
  };

  let create = (): Js.Promise.t(fresh_t) => {
    let algorithm = {
      "name": "RSA-OAEP",
      "modulusLength": 4096,
      "publicExponent": Uint8Array.make([|1, 0, 1|]),
      "hash": "SHA-512",
    };
    let extractable = true;
    let keyUsages = [|"wrapKey", "unwrapKey"|];
    Js.Promise.(
      generateKeyPair_impl(algorithm, extractable, keyUsages)
      |> then_((key_pair: crypto_key_pair) => {
           let private_key = PrivateKey(key_pair.private_key);
           let public_key = PublicKey(key_pair.public_key);
           resolve({private_key, public_key});
         })
    );
  };
};

module VerificationKeyPair = {
  open WebCryptoAPI;

  type public_key_t =
    | PublicKey(crypto_key);
  type private_key_t =
    | PrivateKey(crypto_key);

  type fresh_t = {
    private_key: private_key_t,
    public_key: public_key_t,
  };

  let create = () => {
    let algorithm = {
      "name": "RSA-PSS",
      "modulusLength": 4096,
      "publicExponent": Uint8Array.make([|1, 0, 1|]),
      "hash": "SHA-512",
    };
    let extractable = true;
    let keyUsages = [|"sign", "verify"|];
    Js.Promise.(
      generateKeyPair_impl(algorithm, extractable, keyUsages)
      |> then_((key_pair: crypto_key_pair) => {
           let private_key = PrivateKey(key_pair.private_key);
           let public_key = PublicKey(key_pair.public_key);
           resolve({private_key, public_key});
         })
    );
  };
};

module EphemeralKey = {
  open WebCryptoAPI;

  type key_t =
    | EphemeralKey(crypto_key);

  type message_iv_t =
    | MessageIv(Uint8Array.t);

  type fresh_t = {
    ephemeral_key: key_t,
    message_encryption_iv: message_iv_t,
  };

  let create = () => {
    let message_encryption_iv =
      MessageIv(getRandomValues_impl(Uint8Array.fromLength(12)));
    let algorithm = {"name": "AES-GCM", "length": "256"};
    let extractable = true;
    let keyUsages = [|"encrypt", "decrypt"|];
    Js.Promise.(
      generateKey_impl(algorithm, extractable, keyUsages)
      |> then_(ephemeral_key => {
           let ephemeral_key = EphemeralKey(ephemeral_key);
           resolve({ephemeral_key, message_encryption_iv});
         })
    );
  };
};

module Operations = {
  open WebCryptoAPI;
  open MasterKey;
  open DerivedKey;
  open EphemeralKey;

  let wrap_master_key =
      (
        DerivedKey(derived_key),
        MasterIv(encryption_iv),
        MasterKey(master_key),
      ) => {
    let format = "raw";
    let key = master_key;
    let wrapping_key = derived_key;
    let wrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    wrapKey_impl(format, key, wrapping_key, wrapAlgo);
  };

  let unwrap_master_key =
      (
        DerivedKey(derived_key),
        MasterIv(encryption_iv),
        encrypted_master_key: ArrayBuffer.t,
      ) => {
    let format = "raw";
    let wrappedKey = encrypted_master_key;
    let unwrappingKey = derived_key;
    let unwrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    let unwrappedKeyAlgo = {"name": "AES-GCM"};
    let extractable = true;
    let keyUsages = [|"wrapKey", "unwrapKey"|];
    Js.Promise.(
      unwrapKey_impl(
        format,
        wrappedKey,
        unwrappingKey,
        unwrapAlgo,
        unwrappedKeyAlgo,
        extractable,
        keyUsages,
      )
      |> then_(master_key => resolve(MasterKey(master_key)))
    );
  };

  let wrap_protection_private_key =
      (
        MasterKey(master_key),
        ProtectionIv(encryption_iv),
        ProtectionKeyPair.PrivateKey(private_key),
      ) => {
    let format = "pkcs8";
    let key = private_key;
    let wrappingKey = master_key;
    let wrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    wrapKey_impl(format, key, wrappingKey, wrapAlgo);
  };

  let unwrap_protection_private_key =
      (
        MasterKey(master_key),
        ProtectionIv(encryption_iv),
        encrypted_private_key: ArrayBuffer.t,
      ) => {
    let format = "pkcs8";
    let wrappedKey = encrypted_private_key;
    let unwrappingKey = master_key;
    let unwrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    let unwrappedKeyAlgo = {"name": "RSA-OAEP", "hash": "SHA-512"};
    let extractable = true;
    let keyUsages = [|"unwrapKey"|];
    Js.Promise.(
      unwrapKey_impl(
        format,
        wrappedKey,
        unwrappingKey,
        unwrapAlgo,
        unwrappedKeyAlgo,
        extractable,
        keyUsages,
      )
      |> then_(private_key =>
           resolve(ProtectionKeyPair.PrivateKey(private_key))
         )
    );
  };

  let wrap_verification_private_key =
      (
        MasterKey(master_key),
        VerificationIv(encryption_iv),
        VerificationKeyPair.PrivateKey(private_key),
      ) => {
    let format = "pkcs8";
    let key = private_key;
    let wrappingKey = master_key;
    let wrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    wrapKey_impl(format, key, wrappingKey, wrapAlgo);
  };

  let unwrap_verification_private_key =
      (
        MasterKey(master_key),
        VerificationIv(encryption_iv),
        encrypted_private_key,
      ) => {
    let format = "pkcs8";
    let wrappedKey = encrypted_private_key;
    let unwrappingKey = master_key;
    let unwrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    let unwrappedKeyAlgo = {"name": "RSA-OAEP", "hash": "SHA-512"};
    let extractable = true;
    let keyUsages = [|"unwrapKey"|];
    Js.Promise.(
      unwrapKey_impl(
        format,
        wrappedKey,
        unwrappingKey,
        unwrapAlgo,
        unwrappedKeyAlgo,
        extractable,
        keyUsages,
      )
      |> then_(private_key =>
           resolve(VerificationKeyPair.PrivateKey(private_key))
         )
    );
  };

  let wrap_ephemeral_key =
      (ProtectionKeyPair.PublicKey(public_key), EphemeralKey(ephemeral_key)) => {
    let format = "raw";
    let key = ephemeral_key;
    let wrappingKey = public_key;
    let wrapAlgo = {"name": "RSA-OAEP"};
    wrapKey_impl(format, key, wrappingKey, wrapAlgo);
  };

  let unwrap_ephemeral_key =
      (
        ProtectionKeyPair.PrivateKey(private_key),
        encrypted_ephemeral_key: ArrayBuffer.t,
      ) => {
    let format = "raw";
    let wrappedKey = encrypted_ephemeral_key;
    let unwrappingKey = private_key;
    let unwrapAlgo = {"name": "RSA-OAEP"};
    let unwrappedKeyAlgo = {"name": "AES-GCM"};
    let extractable = true;
    let keyUsages = [|"encrypt", "decrypt"|];
    Js.Promise.(
      unwrapKey_impl(
        format,
        wrappedKey,
        unwrappingKey,
        unwrapAlgo,
        unwrappedKeyAlgo,
        extractable,
        keyUsages,
      )
      |> then_(ephemeral_key => {resolve(EphemeralKey(ephemeral_key))})
    );
  };

  let encrypt_data =
      (
        EphemeralKey(ephemeral_key),
        MessageIv(encryption_iv),
        data: ArrayBuffer.t,
      ) => {
    let algorithm = {"name": "AES-GCM", "iv": encryption_iv};
    let key = ephemeral_key;
    encrypt_impl(algorithm, key, data);
  };

  let decrypt_data =
      (
        EphemeralKey(ephemeral_key),
        MessageIv(encryption_iv),
        data: ArrayBuffer.t,
      ) => {
    let algorithm = {"name": "AES-GCM", "iv": encryption_iv};
    let key = ephemeral_key;
    decrypt_impl(algorithm, key, data);
  };

  let sign_data = (VerificationKeyPair.PrivateKey(key), data: ArrayBuffer.t) => {
    let algorithm = {"name": "RSA-PSS", "saltLength": 64};
    sign_impl(algorithm, key, data);
  };

  let verify_data =
      (
        VerificationKeyPair.PublicKey(key),
        signature: ArrayBuffer.t,
        data: ArrayBuffer.t,
      ) => {
    let algorithm = {"name": "RSA-PSS", "saltLength": 64};
    verify_impl(algorithm, key, signature, data);
  };
};
