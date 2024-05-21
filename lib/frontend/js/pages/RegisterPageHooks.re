open Promise_syntax;
open React.Event;
open RegisterPageHooksCore;

let useValidatedField = validateFn => {
  let (value, setValue) = React.useState(() => "");

  let onChange =
    React.useCallback0((event: Form.t) => {
      Form.preventDefault(event);
      setValue(_ => Form.target(event)##value);
    });

  let (validation, setValidation) = React.useState(() => validateFn(value));

  let (prevValue, setPrevValue) = React.useState(() => value);
  if (value !== prevValue) {
    setPrevValue(_ => value);
    setValidation(_ => validateFn(value));
  };

  {value, onChange, validation, setValidation};
};

let useUsername = () => {
  useValidatedField(ValidationUsername.validate);
};

let usePassword = (~username: string) => {
  useValidatedField(ValidationPassword.validate(username));
};

let useConfirm = (~password: string) => {
  useValidatedField(ValidationPasswordConfirm.validate(password));
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
  let register = SessionHook.useRegister();

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
