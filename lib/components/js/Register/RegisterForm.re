open React.Event;

let validateUsername = username => {
  ValidationUsername.validate(username);
};

let validatePassword = (username, password) => {
  ValidationPassword.validate(username, password);
};

let validateConfirm = (password, confirmPassword) => {
  ValidationPasswordConfirm.validate(password, confirmPassword);
};

[@react.component]
let make =
    (
      ~formState: RegisterFormState.t,
      ~nextRegisterStep,
      ~generateState: RegisterGenerateState.t,
    ) => {
  let username = formState.username;
  let password = formState.password;
  let confirmPassword = formState.confirmPassword;

  let onUsernameChange = event => {
    formState.setUsername(_ => Form.target(event)##value);
  };
  let onPasswordChange = event => {
    formState.setPassword(_ => Form.target(event)##value);
  };
  let onConfirmPasswordChange = event => {
    formState.setConfirmPassword(_ => Form.target(event)##value);
  };

  let usernameState =
    React.useMemo1(_ => validateUsername(username), [|username|]);
  let passwordState =
    React.useMemo2(
      _ => validatePassword(username, password),
      (username, password),
    );
  let confirmState =
    React.useMemo2(
      _ => validateConfirm(password, confirmPassword),
      (password, confirmPassword),
    );

  let onSubmit =
    React.useCallback3(
      event => {
        Form.preventDefault(event);
        let allowUsername = ValidationUsername.allow(usernameState);
        let allowPassword = ValidationPassword.allow(passwordState);
        let allowConfirm = ValidationPasswordConfirm.allow(confirmState);
        if (allowUsername && allowPassword && allowConfirm) {
          open RegisterGenerateState;
          open Vault.PromiseLet;
          let _ = {
            let clientRandom = Vault.ClientRandom.create();
            let* saltBuffer = Vault.Salt.compute_digest(clientRandom);
            let* freshDerivedKey =
              Vault.DerivedKey.create(password, saltBuffer);

            generateState.setClientRandom(_ => Some(clientRandom));
            generateState.setDerivedKey(_ => Some(freshDerivedKey));

            nextRegisterStep();
            resolve();
          };
          ();
        };
      },
      (usernameState, passwordState, confirmState),
    );

  <AuthForm
    register=true
    username
    password
    confirmPassword
    onUsernameChange
    onPasswordChange
    onConfirmPasswordChange
    usernameState
    passwordState
    confirmState
    onSubmit
  />;
};
