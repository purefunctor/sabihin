open React.Event;

let validateUsername = username =>
  if (String.length(username) > 0) {
    Some(Validate.Username.validate(username));
  } else {
    None;
  };

let validatePassword = (username, password) =>
  if (String.length(password) > 0) {
    Some(Validate.Password.validate(username, password));
  } else {
    None;
  };

let validateConfirm = (password, confirmPassword) =>
  if (String.length(confirmPassword) > 0) {
    Some(Validate.Confirm.validate(password, confirmPassword));
  } else {
    None;
  };

[@react.component]
let make = (~formState: RegisterFormState.t, ~nextRegisterStep) => {
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

  let onSubmit = event => {
    Form.preventDefault(event);
    nextRegisterStep();
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
