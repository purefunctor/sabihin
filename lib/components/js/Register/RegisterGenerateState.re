type package_t = {
  encrypted_master_key: string,
  master_key_iv: string,
  encrypted_protection_key: string,
  protection_key_iv: string,
  encrypted_verification_key: string,
  verification_key_iv: string,
};

type t = {
  package: package_t,
  setPackage: (package_t => package_t) => unit,
};

let use = () => {
  let (package, setPackage) =
    React.useState(() =>
      {
        encrypted_master_key: "",
        master_key_iv: "",
        encrypted_protection_key: "",
        protection_key_iv: "",
        encrypted_verification_key: "",
        verification_key_iv: "",
      }
    );
  {package, setPackage};
};

type keys_t =
  | Master
  | Derived
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
        | Master => Keys(Derived)
        | Derived => Keys(Protection)
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
