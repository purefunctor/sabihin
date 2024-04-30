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
  React.useCallback2(
    event => {
      Form.preventDefault(event);

      let allowUsername = ValidationUsername.allow(username.validation);
      let allowPassword = ValidationPasswordBasic.allow(password.validation);

      if (allowUsername && allowPassword) {
        Js.Console.log((username, password));
      };
    },
    (username, password),
  );
};
