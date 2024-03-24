[@react.component]
let make =
    (
      ~registerState: RegisterState.t,
      ~generateState: RegisterGenerateState.t,
      ~nextRegisterStep,
    ) => {
  let _ = generateState;
  let _ = nextRegisterStep;

  React.useEffect0(() => {
    open Vault;
    open Vault.PromiseLet;

    let _ = {
      Js.Console.log("Generating Master Key");
      let* fresh_master_key = MasterKey.create();
      Js.Console.log(fresh_master_key);

      Js.Console.log("Generating Derived Key");
      let* fresh_derived_key =
        DerivedKey.create(
          registerState.password,
          fresh_master_key.salt_buffer,
        );
      Js.Console.log(fresh_derived_key);

      // Js.Console.log("Generating Protection Key Pair");
      // let* fresh_protection_key_pair = ProtectionKeyPair.create();
      // Js.Console.log(fresh_protection_key_pair);

      // Js.Console.log("Generating Verification Key Pair");
      // let* fresh_verification_key_pair = VerificationKeyPair.create();
      // Js.Console.log(fresh_verification_key_pair);

      Js.Console.log("Wrapping Master Key");
      let* wrapped_master_key =
        Operations.wrap_master_key(
          fresh_derived_key.derived_encryption_key,
          fresh_derived_key.master_encryption_iv,
          fresh_master_key.master_key,
        );
      let wrapped_master_key =
        Base64Utils.array_buffer_to_base64(wrapped_master_key);
      Js.Console.log(wrapped_master_key);

      // Js.Console.log("Wrapping Private Protection Key");
      // let* wrapped_protection_private_key =
      //   Operations.wrap_protection_private_key(
      //     fresh_master_key.master_key,
      //     fresh_master_key.protection_key_iv,
      //     fresh_protection_key_pair.private_key,
      //   );
      // let wrapped_protection_private_key =
      //   Base64Utils.array_buffer_to_base64(wrapped_protection_private_key);
      // Js.Console.log(wrapped_protection_private_key);

      // Js.Console.log("Export Public Protection Key");
      // let* exported_protection_public_key =
      //   Operations.export_protection_public_key(
      //     fresh_protection_key_pair.public_key,
      //   );
      // let exported_protection_public_key =
      //   Base64Utils.array_buffer_to_base64(exported_protection_public_key);
      // Js.Console.log(exported_protection_public_key);

      // Js.Console.log("Wrapping Private Verification Key");
      // let* wrapped_verification_private_key =
      //   Operations.wrap_verification_private_key(
      //     fresh_master_key.master_key,
      //     fresh_master_key.verification_key_iv,
      //     fresh_verification_key_pair.private_key,
      //   );
      // let wrapped_verification_private_key =
      //   Base64Utils.array_buffer_to_base64(wrapped_verification_private_key);
      // Js.Console.log(wrapped_verification_private_key);

      // Js.Console.log("Export Public Verification Key");
      // let* exported_verification_public_key =
      //   Operations.export_verification_public_key(
      //     fresh_verification_key_pair.public_key,
      //   );
      // let exported_verification_public_key =
      //   Base64Utils.array_buffer_to_base64(exported_verification_public_key);
      // Js.Console.log(exported_verification_public_key);

      resolve();
    };

    None;
  });

  <div className="register-generate-content">
    <span className="josefin-sans-title generate-title">
      {React.string("Making Your Keys")}
    </span>
    // <Icons.RingResize size="6rem" className="color-primary" />
    <div className="register-generate-steps">
      <span className="poppins-regular register-generate-step">
        {React.string("Verification Key Pair")}
        <Icons.RingResize size="1rem" className="color-primary" />
      </span>
      {[|"Master Key", "Derived Key", "Protection Key Pair"|]
       |> Belt.Array.reverse
       |> Array.map(step => {
            <span
              className="poppins-regular register-generate-step color-offwhite">
              {React.string(step)}
              <Icons.CheckFill size="1rem" className="color-success" />
            </span>
          })
       |> React.array}
    </div>
    <p className="poppins-regular register-generate-hint">
      {React.string(
         "Once finished, your encrypted private keys, public keys, and other metadata will be submitted to the server to finalize your registration.",
       )}
    </p>
  </div>;
};
