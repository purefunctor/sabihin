[@react.component]
let make =
    (
      ~formState: RegisterFormState.t,
      ~generateState: RegisterGenerateState.t,
      ~nextRegisterStep,
    ) => {
  let (generateStep, nextGenerateStep) = RegisterGenerateState.useStep();

  React.useEffect1(
    () => {
      open Vault.PromiseLet;

      let _ = {
        let* fresh_master_key = Vault.MasterKey.create();
        nextGenerateStep();

        let* fresh_derived_key =
          Vault.DerivedKey.create(
            formState.password,
            fresh_master_key.salt_buffer,
          );
        nextGenerateStep();

        let* fresh_protection_keys = Vault.ProtectionKeyPair.create();
        nextGenerateStep();

        let* fresh_verification_keys = Vault.VerificationKeyPair.create();
        nextGenerateStep();

        let* wrapped_master_key =
          Vault.Operations.wrap_master_key(
            fresh_derived_key.derived_encryption_key,
            fresh_derived_key.master_key_iv,
            fresh_master_key.master_key,
          );
        let* wrapped_protection_private_key =
          Vault.Operations.wrap_protection_private_key(
            fresh_master_key.master_key,
            fresh_master_key.protection_key_iv,
            fresh_protection_keys.private_key,
          );
        let* wrapped_verification_private_key =
          Vault.Operations.wrap_verification_private_key(
            fresh_master_key.master_key,
            fresh_master_key.verification_key_iv,
            fresh_verification_keys.private_key,
          );
        nextGenerateStep();

        let encrypted_master_key =
          Vault.Base64Utils.array_buffer_to_base64(wrapped_master_key);
        let master_key_iv = {
          let MasterIv(master_key_iv) = fresh_derived_key.master_key_iv;
          Vault.Base64Utils.uint8array_to_base64(master_key_iv);
        };

        let encrypted_protection_key =
          Vault.Base64Utils.array_buffer_to_base64(
            wrapped_protection_private_key,
          );
        let* exported_protection_key = {
          let* exported_protection_key =
            Vault.Operations.export_protection_public_key(
              fresh_protection_keys.public_key,
            );
          resolve(
            Vault.Base64Utils.array_buffer_to_base64(exported_protection_key),
          );
        };
        let protection_key_iv = {
          let ProtectionIv(protection_key_iv) =
            fresh_master_key.protection_key_iv;
          Vault.Base64Utils.uint8array_to_base64(protection_key_iv);
        };

        let encrypted_verification_key =
          Vault.Base64Utils.array_buffer_to_base64(
            wrapped_verification_private_key,
          );
        let* exported_verification_key = {
          let* exported_verification_key =
            Vault.Operations.export_verification_public_key(
              fresh_verification_keys.public_key,
            );
          resolve(
            Vault.Base64Utils.array_buffer_to_base64(
              exported_verification_key,
            ),
          );
        };
        let verification_key_iv = {
          let VerificationIv(verification_key_iv) =
            fresh_master_key.verification_key_iv;
          Vault.Base64Utils.uint8array_to_base64(verification_key_iv);
        };
        nextGenerateStep();

        generateState.setPackage(_ =>
          {
            encrypted_master_key,
            master_key_iv,
            encrypted_protection_key,
            exported_protection_key,
            protection_key_iv,
            encrypted_verification_key,
            exported_verification_key,
            verification_key_iv,
          }
        );

        let _ = Js.Global.setTimeout(~f=nextRegisterStep, 1000);

        resolve();
      };

      None;
    },
    [||],
  );

  <div className="register-generate-content">
    <span className="josefin-sans-title generate-title">
      {React.string("Making Your Keys")}
    </span>
    <RegisterGenerateStep generateStep />
    <p className="poppins-regular register-generate-hint">
      {React.string(
         "Once finished, your encrypted private keys, public keys, and other metadata will be submitted to the server to finalize your registration.",
       )}
    </p>
  </div>;
};
