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

type step_t =
  | Master
  | Derived
  | Protection
  | Verification
  | EncryptingKeys
  | ExportingKeys;

let useStep = () => {
  React.useReducer((step: step_t, ()) => {
    switch (step) {
    | Master => Derived
    | Derived => Protection
    | Protection => Verification
    | Verification => EncryptingKeys
    | EncryptingKeys => ExportingKeys
    | ExportingKeys => raise(Failure("No more states!"))
    }
  });
};
