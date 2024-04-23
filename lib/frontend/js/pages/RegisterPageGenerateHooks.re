open RegisterPageHooks;
open Types_universal.Definitions_t;

module Step = {
  type t =
    | MasterKey
    | ProtectionKey
    | VerificationKey
    | EncryptingKeys
    | ExportingKeys
    | Finished;

  type kind =
    | Done
    | Current
    | Final;

  let text = [|
    "Creating Master Key",
    "Creating Protection Key Pair",
    "Creating Verification Key Pair",
    "Encrypting Keys",
    "Exporting Keys",
    "Finished!",
  |];

  let toIndex = (step: t) => {
    switch (step) {
    | MasterKey => 0
    | ProtectionKey => 1
    | VerificationKey => 2
    | EncryptingKeys => 3
    | ExportingKeys => 4
    | Finished => 5
    };
  };

  let toArray = (step: t) => {
    let total = Array.length(text);
    let final = step |> toIndex;
    Array.sub(text, 0, final + 1)
    |> Array.mapi((index, text) => {
         let kind =
           if (index < final) {
             Done;
           } else if (index == total - 1) {
             Final;
           } else {
             Current;
           };
         (kind, text);
       });
  };
};

let takeSomeTime = () => {
  Js.Promise.make((~resolve, ~reject as _) => {
    let unit = ();
    let _ = Js.Global.setTimeout(~f=() => {resolve(. unit)}, 500);
    ();
  });
};

let generateKeys = (~generate: generate, ~setStep: Step.t => unit) => {
  open Js.Promise;
  open Vault;

  let ( let* ) = (f, x) => then_(x, f);

  let client_random_value =
    generate.clientRandom |> Base64Utils.uint8array_to_base64;

  let* salt_buffer = Salt.compute_digest(generate.clientRandom);
  let fresh_derived_key = generate.derivedKey;

  let* () = takeSomeTime();
  setStep(Step.MasterKey);
  let* fresh_master_key = MasterKey.create(salt_buffer);

  let* () = takeSomeTime();
  setStep(Step.ProtectionKey);
  let* fresh_protection_keys = ProtectionKeyPair.create();

  let* () = takeSomeTime();
  setStep(Step.VerificationKey);
  let* fresh_verification_keys = VerificationKeyPair.create();

  let* () = takeSomeTime();
  setStep(Step.EncryptingKeys);
  let* wrapped_master_key =
    Operations.wrap_master_key(
      fresh_derived_key.derived_encryption_key,
      fresh_derived_key.master_key_iv,
      fresh_master_key.master_key,
    );
  let* wrapped_protection_private_key =
    Operations.wrap_protection_private_key(
      fresh_master_key.master_key,
      fresh_master_key.protection_key_iv,
      fresh_protection_keys.private_key,
    );
  let* wrapped_verification_private_key =
    Operations.wrap_verification_private_key(
      fresh_master_key.master_key,
      fresh_master_key.verification_key_iv,
      fresh_verification_keys.private_key,
    );

  let* () = takeSomeTime();
  setStep(Step.ExportingKeys);
  let encrypted_master_key =
    Base64Utils.array_buffer_to_base64(wrapped_master_key);
  let master_key_iv = {
    let MasterIv(master_key_iv) = fresh_derived_key.master_key_iv;
    Base64Utils.uint8array_to_base64(master_key_iv);
  };

  let encrypted_protection_key =
    Base64Utils.array_buffer_to_base64(wrapped_protection_private_key);
  let* exported_protection_key = {
    let* exported_protection_key =
      Operations.export_protection_public_key(
        fresh_protection_keys.public_key,
      );
    resolve(Base64Utils.array_buffer_to_base64(exported_protection_key));
  };
  let protection_key_iv = {
    let ProtectionIv(protection_key_iv) = fresh_master_key.protection_key_iv;
    Base64Utils.uint8array_to_base64(protection_key_iv);
  };

  let encrypted_verification_key =
    Base64Utils.array_buffer_to_base64(wrapped_verification_private_key);
  let* exported_verification_key = {
    let* exported_verification_key =
      Operations.export_verification_public_key(
        fresh_verification_keys.public_key,
      );
    resolve(Base64Utils.array_buffer_to_base64(exported_verification_key));
  };
  let verification_key_iv = {
    let VerificationIv(verification_key_iv) =
      fresh_master_key.verification_key_iv;
    Base64Utils.uint8array_to_base64(verification_key_iv);
  };

  let* () = takeSomeTime();
  setStep(Step.Finished);
  let keysPayload = {
    client_random_value,
    encrypted_master_key,
    master_key_iv,
    encrypted_protection_key,
    exported_protection_key,
    protection_key_iv,
    encrypted_verification_key,
    exported_verification_key,
    verification_key_iv,
  };

  Js.Console.log(keysPayload);

  resolve();
};

let fakeGenerateKeys = (~generate as _: generate, ~setStep: Step.t => unit) => {
  open Js.Promise;
  let ( let* ) = (f, x) => then_(x, f);

  setStep(Step.MasterKey);
  let* () = takeSomeTime();
  let* () = takeSomeTime();

  setStep(Step.ProtectionKey);
  let* () = takeSomeTime();
  let* () = takeSomeTime();

  setStep(Step.VerificationKey);
  let* () = takeSomeTime();
  let* () = takeSomeTime();

  setStep(Step.EncryptingKeys);
  let* () = takeSomeTime();
  let* () = takeSomeTime();

  setStep(Step.ExportingKeys);
  let* () = takeSomeTime();
  let* () = takeSomeTime();

  setStep(Step.Finished);
  let* () = takeSomeTime();
  let* () = takeSomeTime();

  resolve();
};

let useGenerateKeys = (~generate: generate, ~toSubmit as _) => {
  let isGenerating = React.useRef(false);

  let (step, setStep) = React.useState(() => Step.MasterKey);
  let setStep = step => setStep(_ => step);

  React.useEffect0(() => {
    if (!isGenerating.current) {
      ignore(
        fakeGenerateKeys(~generate, ~setStep),
        // ignore(generateKeys(~generate, ~setStep));
      );
    };
    None;
  });

  Step.toArray(step);
};
