open React.Event;
open RegisterPageHooksCore;

let useField = RegisterPageHooks.useField;

let useUsername = () => {
  let (value, onChange) = useField();
  let (validation, setValidation) =
    React.useState(() => ValidationUsername.validate(value));
  React.useEffect1(
    () => {
      setValidation(_ => ValidationUsername.validate(value));
      None;
    },
    [|value|],
  );
  {value, onChange, validation, setValidation};
};

let usePassword = () => {
  let (value, onChange) = useField();
  let (validation, setValidation) =
    React.useState(() => ValidationPasswordBasic.validate(value));
  React.useEffect1(
    () => {
      setValidation(_ => ValidationPasswordBasic.validate(value));
      None;
    },
    [|value|],
  );
  {value, onChange, validation, setValidation};
};

let useFormSubmit =
    (
      ~username: fieldHook(ValidationUsername.t),
      ~password: fieldHook(ValidationPasswordBasic.t),
    ) => {
  let login = Session.useLogin();

  React.useCallback2(
    event => {
      Form.preventDefault(event);

      let allowUsername = ValidationUsername.allow(username.validation);
      let allowPassword = ValidationPasswordBasic.allow(password.validation);

      if (allowUsername && allowPassword) {
        let username = username.value;
        let password = password.value;
        ignore(login(~username, ~password));
      };
    },
    (username, password),
  );
};
