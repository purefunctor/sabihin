open React.Event;

[@react.component]
let make = (~toGenerate: RegisterHooks.Step.toGenerate) => {
  let (form, setForm) = RegisterHooks.Form.use();

  let username = form.username;
  let password = form.password;
  let confirmPassword = form.confirmPassword;

  let onUsernameChange = event => {
    setForm(prevForm => {...prevForm, username: Form.target(event)##value});
  };
  let onPasswordChange = event => {
    setForm(prevForm => {...prevForm, password: Form.target(event)##value});
  };
  let onConfirmPasswordChange = event => {
    setForm(prevForm =>
      {...prevForm, confirmPassword: Form.target(event)##value}
    );
  };

  let usernameState =
    React.useMemo1(
      () => {ValidationUsername.validate(username)},
      [|username|],
    );
  let passwordState =
    React.useMemo1(
      () => {ValidationPassword.validate(username, password)},
      [|username, password|],
    );
  let confirmState =
    React.useMemo1(
      () => {ValidationPasswordConfirm.validate(password, confirmPassword)},
      [|password, confirmPassword|],
    );

  let onSubmit = event => {
    Form.preventDefault(event);
    let allowUsername = ValidationUsername.allow(usernameState);
    let allowPassword = ValidationPassword.allow(passwordState);
    let allowConfirm = ValidationPasswordConfirm.allow(confirmState);

    if (allowUsername && allowPassword && allowConfirm) {
      let _ = {
        open RegisterHooks.Step;
        open Vault.PromiseLet;

        // TODO: Insert actual logic here.
        let publicId = "publicIdExample";
        let clientRandom = Vault.ClientRandom.create();

        let* saltBuffer = Vault.Salt.compute_digest(clientRandom);
        let* derivedKey = Vault.DerivedKey.create(password, saltBuffer);

        toGenerate({publicId, clientRandom, derivedKey});
        resolve();
      };
      ();
    };
  };

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
