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
        open Types_js.Defs_bs;
        open Vault;
        open PromiseLet;

        let clientRandom = ClientRandom.create();
        let* saltBuffer = Salt.compute_digest(clientRandom);
        let* derivedKey = DerivedKey.create(password, saltBuffer);

        let auth_token = Salt.toHash(derivedKey.hashed_authentication_key);
        let body =
          write_register_payload_t({username, auth_token})
          |> Js.Json.stringify
          |> Fetch.BodyInit.make;

        let* registerResponse = {
          let method_ = Fetch.Post;
          let headers =
            Fetch.HeadersInit.make({"Content-Type": "application/json"});
          let requestInit =
            Fetch.RequestInit.make(~method_, ~headers, ~body, ());
          Fetch.fetchWithInit("/api/register", requestInit);
        };
        let* registerJson = Fetch.Response.json(registerResponse);

        let registerResponse = read_register_response_t(registerJson);
        let publicId = registerResponse.public_id;

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
