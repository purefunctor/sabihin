open React.Event;

[@react.component]
let make = () => {
  let (username, setUsername) = React.useState(() => "");
  let (password, setPassword) = React.useState(() => "");
  let (confirmPassword, setConfirmPassword) = React.useState(() => "");

  let (usernameState, setUsernameState) = React.useState(() => None);
  let (passwordState, setPasswordState) = React.useState(() => None);
  let (confirmState, setConfirmState) = React.useState(() => None);

  let onUsernameChange = event => {
    let value = Form.target(event)##value;
    if (String.length(value) > 0) {
      switch (Validate.Username.validate(value)) {
      | Error(e) => setUsernameState(_ => Some(e))
      | Ok(_) => setUsernameState(_ => None)
      };
    } else {
      setUsernameState(_ => None);
    };
    setUsername(_ => value);
  };
  let onPasswordChange = event => {
    let value = Form.target(event)##value;
    if (String.length(value) > 0) {
      setPasswordState(_ =>
        Some(Validate.Password.validate(username, value))
      );
    } else {
      setPasswordState(_ => None);
    };
    setPassword(_ => value);
  };
  let onConfirmPasswordChange = event => {
    let value = Form.target(event)##value;
    if (String.length(value) > 0) {
      setConfirmState(_ => Some(Validate.Confirm.validate(password, value)));
    } else {
      setConfirmState(_ => None);
    };
    setConfirmPassword(_ => Form.target(event)##value);
  };

  let onSubmit = event => {
    Form.preventDefault(event);
    let target = Form.target(event);
    let username = target##username##value;
    let password = target##password##value;
    let confirmPassword = target##confirmPassword##value;
    Js.Console.log2("Username:", username);
    Js.Console.log2("Password:", password);
    Js.Console.log2("Confirm Password:", confirmPassword);
  };

  <AuthCore
    register=true
    username
    onUsernameChange
    password
    onPasswordChange
    confirmPassword
    onConfirmPasswordChange
    onSubmit
    usernameState
    passwordState
    confirmState
  />;
};
