open Promise_syntax;
open React.Event;
open RegisterPageHooksCore;

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

let usePassword = (~username: string) => {
  let (value, onChange) = useField();
  let (validation, setValidation) =
    React.useState(() => ValidationPassword.validate(username, value));
  React.useEffect2(
    () => {
      setValidation(_ => ValidationPassword.validate(username, value));
      None;
    },
    (username, value),
  );
  {value, onChange, validation, setValidation};
};

let useConfirm = (~password: string) => {
  let (value, onChange) = useField();
  let (validation, setValidation) =
    React.useState(() => ValidationPasswordConfirm.validate(password, value));
  React.useEffect2(
    () => {
      setValidation(_ => ValidationPasswordConfirm.validate(password, value));
      None;
    },
    (password, value),
  );
  {value, onChange, validation, setValidation};
};

let submitCore =
    (
      ~register:
         (~username: string, ~password: string) =>
         Js.Promise.t(ApiCore.registerResult),
      ~username: fieldHook(ValidationUsername.t),
      ~password: fieldHook(ValidationPassword.t),
    ) => {
  let+ submitResult = {
    let username = username.value;
    let password = password.value;
    register(~username, ~password);
  };
  switch (submitResult) {
  | Ok(_) => ReasonReactRouter.push("/get-started")
  | Error(e) =>
    let apiError = ApiCore.registerErrorToString(e);
    username.setValidation(_ => Validated(ApiError(apiError)));
  };
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
        ignore(submitCore(~register, ~username, ~password));
      };
    },
    (username, password, confirm),
  );
};
