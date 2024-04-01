open Js.Typed_array;
open Types_js.Defs_t;
open Vault;

module Form = {
  type t = {
    username: string,
    password: string,
    confirmPassword: string,
  };

  let use = () => {
    React.useState(() => {username: "", password: "", confirmPassword: ""});
  };
};

module Generate = {
  type t = register_keys_payload_t;

  let use = () => {
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
  };

  type keys_t =
    | Master
    | Protection
    | Verification
    | EncryptingKeys
    | ExportingKeys;

  let keysToInt: keys_t => int =
    fun
    | Master => 0
    | Protection => 1
    | Verification => 2
    | EncryptingKeys => 3
    | ExportingKeys => 4;

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
};

module Step = {
  type generate = {
    publicId: string,
    clientRandom: Uint8Array.t,
    derivedKey: DerivedKey.fresh_t,
  };

  type submit = {
    publicId: string,
    keysPayload: register_keys_payload_t,
  };

  type t =
    | Form
    | Generate(generate)
    | Submit(submit);

  type toGenerate = generate => unit;
  type toSubmit = submit => unit;

  let use = () => {
    let (step, setStep) = React.useState(() => Form);
    let toGenerate = generate => setStep(_ => Generate(generate));
    let toSubmit = submit => setStep(_ => Submit(submit));
    (step, toGenerate, toSubmit);
  };
};
