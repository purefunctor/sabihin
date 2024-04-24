open React.Event;
open RegisterPageHooksCore;
open Types_js.Definitions_bs;
open Vault_js;

let useField = () => {
  let (state, setState) = React.useState(() => "");
  let onChange =
    React.useCallback0((event: Form.t) => {
      Form.preventDefault(event);
      setState(_ => Form.target(event)##value);
    });
  (state, onChange);
};

let useUsername = () => {
  let (value, onChange) = useField();
  let validation =
    React.useMemo1(() => ValidationUsername.validate(value), [|value|]);
  {value, onChange, validation};
};

let usePassword = (~username: string) => {
  let (value, onChange) = useField();
  let validation =
    React.useMemo2(
      () => {ValidationPassword.validate(username, value)},
      (username, value),
    );
  {value, onChange, validation};
};

let useConfirm = (~password: string) => {
  let (value, onChange) = useField();
  let validation =
    React.useMemo2(
      () => ValidationPasswordConfirm.validate(password, value),
      (password, value),
    );
  {value, onChange, validation};
};

let useFormSubmit =
    (
      ~username: fieldHook(ValidationUsername.t),
      ~password: fieldHook(ValidationPassword.t),
      ~confirm: fieldHook(ValidationPasswordConfirm.t),
    ) => {
  let register = Session.useRegister();

  React.useCallback3(
    event => {
      Form.preventDefault(event);

      let allowUsername = ValidationUsername.allow(username.validation);
      let allowPassword = ValidationPassword.allow(password.validation);
      let allowConfirm = ValidationPasswordConfirm.allow(confirm.validation);

      if (allowUsername && allowPassword && allowConfirm) {
        let username = username.value;
        let password = password.value;

        let _ = {
          let ( let* ) = (f, x) => Js.Promise.then_(x, f);

          let clientRandom = ClientRandom.create();
          let* saltBuffer = Salt.compute_digest(clientRandom);
          let* derivedKey = DerivedKey.create(password, saltBuffer);

          let auth_token = Salt.toHash(derivedKey.hashed_authentication_key);
          let* registerResult = register({username, auth_token});

          switch (registerResult) {
          | Ok(registerResult) =>
            let publicId = registerResult.public_id;
            Js.Console.log3(publicId, clientRandom, derivedKey);
          | Error(_) => ()
          };

          Js.Promise.resolve();
        };
        ();
      };
    },
    (username, password, confirm),
  );
};
