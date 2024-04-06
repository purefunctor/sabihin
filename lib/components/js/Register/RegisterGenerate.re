module Step = {
  open RegisterHooks.Generate;

  let keysText = [|
    "Master Key",
    "Protection Key",
    "Verification Key",
    "Encrypting Keys",
    "Exporting Keys",
  |];

  let makeStep = (~finished=false, text) => {
    let className = if (finished) {" color-offwhite"} else {""};
    let icon =
      if (finished) {
        <Icons.CheckFill size="1rem" className="color-success" />;
      } else {
        <Icons.RingResize size="1rem" className="color-primary" />;
      };

    <FramerMotion.span
      initial={"y": 100, "opacity": 0.0}
      animate={"y": 0, "opacity": 1.0}
      transition={"ease": "easeOut"}
      className={"poppins-regular register-generate-step" ++ className}
      key=text>
      {React.string(text)}
      icon
    </FramerMotion.span>;
  };

  let useKeys = generateStep => {
    // Note: This is a pure hook that makes use of internal mutability.
    //
    // In summary, it performs the following actions:
    // * Initializes an array of React elements of length n;
    // * For each generateStep change, it sets the state of the
    //   corresponding current (and previous) step in the array
    //   using setUnsafe.
    //
    // This approach ensures that the array need not be constructed
    // from scratch every time generateStep changes. Consequently,
    // this minimizes the number of times the makeStep function is
    // called.
    //
    // Note: The array is copied on each state change to trigger a
    // proper component re-render instead of solely relying on each
    // generateStep change.
    Hooks.useMemoPrevious(
      () => Belt.(Array.make(Array.length(keysText), React.null)),
      keysElements => {
        let keysElements = Belt.Array.copy(keysElements);

        switch (generateStep) {
        | Keys(keys) =>
          let index = keysToInt(keys);
          let text = Belt.Array.getUnsafe(keysText, index);
          let step = makeStep(~finished=false, text);
          Belt.Array.setUnsafe(keysElements, index, step);

          if (index > 0) {
            let prev_index = keysToInt(keys) - 1;
            let prev_text = Belt.Array.getUnsafe(keysText, prev_index);
            let prev_step = makeStep(~finished=true, prev_text);
            Belt.Array.setUnsafe(keysElements, prev_index, prev_step);
          };
        | Finished =>
          let prev_index = Belt.Array.length(keysText) - 1;
          let prev_text = Belt.Array.getUnsafe(keysText, prev_index);
          let prev_step = makeStep(~finished=true, prev_text);
          Belt.Array.setUnsafe(keysElements, prev_index, prev_step);
        };

        keysElements;
      },
      [|generateStep|],
    );
  };

  let useFinal = generateStep => {
    React.useMemo1(
      () => {
        switch (generateStep) {
        | Finished =>
          <FramerMotion.span
            initial={"y": 100, "opacity": 0.0}
            animate={"y": 0, "opacity": 1.0}
            transition={"ease": "easeOut"}
            className="poppins-regular">
            {React.string("Done!")}
          </FramerMotion.span>
        | _ => React.null
        }
      },
      [|generateStep|],
    );
  };

  [@react.component]
  let make = (~generateStep) => {
    let keys = useKeys(generateStep);
    let final = useFinal(generateStep);
    <div className="register-generate-steps">
      {React.array(keys)}
      final
    </div>;
  };
};

let useGenerateKeys =
    (
      generate: RegisterHooks.Step.generate,
      toSubmit: RegisterHooks.Step.toSubmit,
    ) => {
  let publicId = generate.publicId;
  let (generateStep, nextGenerateStep) = RegisterHooks.Generate.use();
  let isGenerating = React.useRef(false);

  let generateInner = () => {
    open Vault;
    open PromiseLet;
    open Types_js.Defs_t;

    let _ = {
      let client_random_value =
        generate.clientRandom |> Base64Utils.uint8array_to_base64;

      let* salt_buffer = Salt.compute_digest(generate.clientRandom);
      let fresh_derived_key = generate.derivedKey;

      let* fresh_master_key = MasterKey.create(salt_buffer);
      nextGenerateStep();

      let* fresh_protection_keys = ProtectionKeyPair.create();
      nextGenerateStep();

      let* fresh_verification_keys = VerificationKeyPair.create();
      nextGenerateStep();

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
      nextGenerateStep();

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
        let ProtectionIv(protection_key_iv) =
          fresh_master_key.protection_key_iv;
        Base64Utils.uint8array_to_base64(protection_key_iv);
      };

      let encrypted_verification_key =
        Base64Utils.array_buffer_to_base64(wrapped_verification_private_key);
      let* exported_verification_key = {
        let* exported_verification_key =
          Operations.export_verification_public_key(
            fresh_verification_keys.public_key,
          );
        resolve(
          Base64Utils.array_buffer_to_base64(exported_verification_key),
        );
      };
      let verification_key_iv = {
        let VerificationIv(verification_key_iv) =
          fresh_master_key.verification_key_iv;
        Base64Utils.uint8array_to_base64(verification_key_iv);
      };
      nextGenerateStep();

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

      let _ =
        Js.Global.setTimeout(
          ~f=() => {toSubmit({publicId, keysPayload})},
          1000,
        );

      resolve();
    };
    ();
  };

  React.useEffect1(
    () => {
      if (!isGenerating.current) {
        isGenerating.current = true;
        generateInner();
      };
      None;
    },
    [||],
  );

  generateStep;
};

[@react.component]
let make =
    (
      ~generate: RegisterHooks.Step.generate,
      ~toSubmit: RegisterHooks.Step.toSubmit,
    ) => {
  let generateStep = useGenerateKeys(generate, toSubmit);

  <div className="register-generate-content">
    <span className="josefin-sans-title generate-title">
      {React.string("Making Your Keys")}
    </span>
    <Step generateStep />
    <p className="poppins-regular register-generate-hint">
      {React.string(
         "Once finished, your encrypted private keys, public keys, and other metadata will be submitted to the server to finalize your registration.",
       )}
    </p>
  </div>;
};
