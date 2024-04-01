open Js.Typed_array;
open Types_js.Defs_bs;

type client_random_t = option(Uint8Array.t);
type derived_key_t = option(Vault.DerivedKey.fresh_t);
type package_t = register_keys_payload_t;

type t = {
  package: package_t,
  setPackage: (package_t => package_t) => unit,
  clientRandom: client_random_t,
  setClientRandom: (client_random_t => client_random_t) => unit,
  derivedKey: derived_key_t,
  setDerivedKey: (derived_key_t => derived_key_t) => unit,
};

let use = () => {
  let (package, setPackage) =
    React.useState(() =>
      {
        encrypted_master_key: "",
        master_key_iv: "",
        encrypted_protection_key: "",
        exported_protection_key: "",
        protection_key_iv: "",
        encrypted_verification_key: "",
        exported_verification_key: "",
        verification_key_iv: "",
      }
    );
  let (clientRandom, setClientRandom) = React.useState(() => None);
  let (derivedKey, setDerivedKey) = React.useState(() => None);
  {
    package,
    setPackage,
    clientRandom,
    setClientRandom,
    derivedKey,
    setDerivedKey,
  };
};

type keys_t =
  | Master
  | Protection
  | Verification
  | EncryptingKeys
  | ExportingKeys;

type step_t =
  | Keys(keys_t)
  | Finished;

let useStep = () => {
  React.useReducer(
    (step: step_t, ()) => {
      switch (step) {
      | Keys(keys) =>
        switch (keys) {
        | Master => Keys(Protection)
        | Protection => Keys(Verification)
        | Verification => Keys(EncryptingKeys)
        | EncryptingKeys => Keys(ExportingKeys)
        | ExportingKeys => Finished
        }
      | Finished => raise(Failure("No more states!"))
      }
    },
    Keys(Master),
  );
};
