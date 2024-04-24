open Js.Typed_array;

module PromiseLet = {
  open Js.Promise;

  let resolve = resolve;

  let ( let* ) = (x, f) => x |> then_(f);
};

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
  external exportKey_impl: (string, crypto_key) => Js.Promise.t(ArrayBuffer.t) =
    "exportKey";

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

  let compute_base = (salt: Uint8Array.t) => {
    assert(Uint8Array.length(salt) == 16);
    let text_encoder = TextEncoder.create();
    let salt_buffer = Uint8Array.make(Array.make(256, 0));

    // Encode first 10 bytes.
    TextEncoder.encodeInto(text_encoder, "sabihin.ph", salt_buffer);

    // Encode next 230 bytes.
    for (index in 10 to 240) {
      Uint8Array.unsafe_set(salt_buffer, index, Char.code('L'));
    };

    // Encode next 16 bytes.
    for (index in 0 to 16) {
      Uint8Array.unsafe_set(
        salt_buffer,
        index + 240,
        Uint8Array.unsafe_get(salt, index),
      );
    };

    salt_buffer;
  };

  let compute_digest = (salt: Uint8Array.t) => {
    digest_impl("SHA-256", compute_base(salt));
  };

  // From: https://developer.mozilla.org/en-US/docs/Web/API/SubtleCrypto/digest
  let toHash: ArrayBuffer.t => string = [%mel.raw
    {|
    (data) => {
      const data_array = Array.from(new Uint8Array(data));
      return data_array.map((c) => c.toString(16).padStart(2, "0")).join("");
    }
  |}
  ];
};

module ClientRandom = {
  open WebCryptoAPI;

  let create = () => {
    getRandomValues_impl(Uint8Array.fromLength(16));
  };
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
    protection_key_iv: protection_iv_t,
    verification_key_iv: verification_iv_t,
  };

  let create = (salt_buffer: ArrayBuffer.t) => {
    open PromiseLet;

    let random_data = getRandomValues_impl(Uint8Array.fromLength(128));
    let protection_key_iv =
      ProtectionIv(getRandomValues_impl(Uint8Array.fromLength(12)));
    let verification_key_iv =
      VerificationIv(getRandomValues_impl(Uint8Array.fromLength(12)));

    let* base_key = {
      let format = "raw";
      let keyData = random_data;
      let algorithm = "HKDF";
      let extractable = false;
      let keyUsages = [|"deriveKey"|];
      importKey_impl(format, keyData, algorithm, extractable, keyUsages);
    };

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

    let* master_key =
      deriveKey_impl(
        algoritm,
        baseKey,
        derivedKeyAlgorithm,
        extractable,
        keyUsages,
      );

    let master_key = MasterKey(master_key);
    resolve({master_key, protection_key_iv, verification_key_iv});
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
    master_key_iv: master_iv_t,
  };

  let create = (password: string, salt_buffer: ArrayBuffer.t) => {
    open PromiseLet;

    let text_encoder = TextEncoder.create();
    let password_raw = TextEncoder.encode(text_encoder, password);
    let master_key_iv =
      MasterIv(getRandomValues_impl(Uint8Array.fromLength(12)));

    let* base_key = {
      let format = "raw";
      let keyData = password_raw;
      let algorithm = "PBKDF2";
      let extractable = false;
      let keyUsages = [|"deriveBits"|];
      importKey_impl(format, keyData, algorithm, extractable, keyUsages);
    };

    let* key_bits = {
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

    let* derived_encryption_key = {
      let derived_encryption_key_raw =
        Uint8Array.fromBuffer(~off=0, ~len=16, key_bits, ());
      let format = "raw";
      let keyData = derived_encryption_key_raw;
      let algorithm = {"name": "AES-GCM"};
      let extractable = false;
      let keyUsages = [|"wrapKey", "unwrapKey"|];
      importKey_impl(format, keyData, algorithm, extractable, keyUsages);
    };

    let* hashed_authentication_key = {
      let derived_authentication_key =
        Uint8Array.fromBuffer(~off=16, ~len=16, key_bits, ());
      digest_impl("SHA-256", derived_authentication_key);
    };

    let derived_encryption_key = DerivedKey(derived_encryption_key);
    resolve({
      derived_encryption_key,
      hashed_authentication_key,
      master_key_iv,
    });
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
    open PromiseLet;

    let algorithm = {
      "name": "RSA-OAEP",
      "modulusLength": 4096,
      "publicExponent": Uint8Array.make([|1, 0, 1|]),
      "hash": "SHA-512",
    };
    let extractable = true;
    let keyUsages = [|"wrapKey", "unwrapKey"|];

    let* key_pair = generateKeyPair_impl(algorithm, extractable, keyUsages);

    let private_key = PrivateKey(key_pair.private_key);
    let public_key = PublicKey(key_pair.public_key);
    resolve({private_key, public_key});
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
    open PromiseLet;

    let algorithm = {
      "name": "RSA-PSS",
      "modulusLength": 4096,
      "publicExponent": Uint8Array.make([|1, 0, 1|]),
      "hash": "SHA-512",
    };
    let extractable = true;
    let keyUsages = [|"sign", "verify"|];

    let* key_pair = generateKeyPair_impl(algorithm, extractable, keyUsages);

    let private_key = PrivateKey(key_pair.private_key);
    let public_key = PublicKey(key_pair.public_key);
    resolve({private_key, public_key});
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
    message_iv: message_iv_t,
  };

  let create = () => {
    open PromiseLet;

    let message_iv =
      MessageIv(getRandomValues_impl(Uint8Array.fromLength(12)));
    let algorithm = {"name": "AES-GCM", "length": "256"};
    let extractable = true;
    let keyUsages = [|"encrypt", "decrypt"|];

    let* ephemeral_key = generateKey_impl(algorithm, extractable, keyUsages);

    let ephemeral_key = EphemeralKey(ephemeral_key);
    resolve({ephemeral_key, message_iv});
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
    open PromiseLet;

    let format = "raw";
    let wrappedKey = encrypted_master_key;
    let unwrappingKey = derived_key;
    let unwrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    let unwrappedKeyAlgo = {"name": "AES-GCM"};
    let extractable = true;
    let keyUsages = [|"wrapKey", "unwrapKey"|];

    let* master_key =
      unwrapKey_impl(
        format,
        wrappedKey,
        unwrappingKey,
        unwrapAlgo,
        unwrappedKeyAlgo,
        extractable,
        keyUsages,
      );

    resolve(MasterKey(master_key));
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
    open PromiseLet;

    let format = "pkcs8";
    let wrappedKey = encrypted_private_key;
    let unwrappingKey = master_key;
    let unwrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    let unwrappedKeyAlgo = {"name": "RSA-OAEP", "hash": "SHA-512"};
    let extractable = true;
    let keyUsages = [|"unwrapKey"|];

    let* private_key =
      unwrapKey_impl(
        format,
        wrappedKey,
        unwrappingKey,
        unwrapAlgo,
        unwrappedKeyAlgo,
        extractable,
        keyUsages,
      );

    resolve(ProtectionKeyPair.PrivateKey(private_key));
  };

  let export_protection_public_key =
      (ProtectionKeyPair.PublicKey(public_key)) => {
    let format = "spki";
    exportKey_impl(format, public_key);
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
    open PromiseLet;

    let format = "pkcs8";
    let wrappedKey = encrypted_private_key;
    let unwrappingKey = master_key;
    let unwrapAlgo = {"name": "AES-GCM", "iv": encryption_iv};
    let unwrappedKeyAlgo = {"name": "RSA-OAEP", "hash": "SHA-512"};
    let extractable = true;
    let keyUsages = [|"unwrapKey"|];

    let* private_key =
      unwrapKey_impl(
        format,
        wrappedKey,
        unwrappingKey,
        unwrapAlgo,
        unwrappedKeyAlgo,
        extractable,
        keyUsages,
      );

    resolve(VerificationKeyPair.PrivateKey(private_key));
  };

  let export_verification_public_key =
      (VerificationKeyPair.PublicKey(public_key)) => {
    let format = "spki";
    exportKey_impl(format, public_key);
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
    open PromiseLet;

    let format = "raw";
    let wrappedKey = encrypted_ephemeral_key;
    let unwrappingKey = private_key;
    let unwrapAlgo = {"name": "RSA-OAEP"};
    let unwrappedKeyAlgo = {"name": "AES-GCM"};
    let extractable = true;
    let keyUsages = [|"encrypt", "decrypt"|];

    let* ephemeral_key =
      unwrapKey_impl(
        format,
        wrappedKey,
        unwrappingKey,
        unwrapAlgo,
        unwrappedKeyAlgo,
        extractable,
        keyUsages,
      );

    resolve(EphemeralKey(ephemeral_key));
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
