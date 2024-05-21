open Promise_syntax;
open React.Event;
open RegisterPageHooksCore;

let useValidatedField = RegisterPageHooks.useValidatedField;

let useUsername = () => {
  useValidatedField(ValidationUsername.validate);
};

let usePassword = () => {
  useValidatedField(ValidationPasswordBasic.validate);
};

let useSubmitCore = () => {
  let login = SessionHook.useLogin();
  let generatedSecretsContext = GeneratedSecretsContext.useContext();

  React.useCallback0(
    (
      ~username: fieldHook(ValidationUsername.t),
      ~password: fieldHook(ValidationPasswordBasic.t),
    ) => {
    let* submitResult = {
      let username = username.value;
      let password = password.value;
      login(~username, ~password);
    };

    switch (submitResult) {
    | Ok(_) =>
      let+ generatedSecrets = generatedSecretsContext.get();
      switch (generatedSecrets) {
      | Some(_) => ReasonReactRouter.push("/inbox")
      | None => ReasonReactRouter.push("/get-started")
      };
    | Error(e) =>
      let apiError = ApiCore.loginErrorToString(e);
      username.setValidation(_ => Validated(ApiError(apiError)));
      resolve();
    };
  });
};

let useFormSubmit =
    (
      ~username: fieldHook(ValidationUsername.t),
      ~password: fieldHook(ValidationPasswordBasic.t),
    ) => {
  let submitCore = useSubmitCore();

  React.useCallback2(
    event => {
      Form.preventDefault(event);

      let allowUsername = ValidationUsername.allow(username.validation);
      let allowPassword = ValidationPasswordBasic.allow(password.validation);

      if (allowUsername && allowPassword) {
        ignore(submitCore(~username, ~password));
      };
    },
    (username, password),
  );
};
