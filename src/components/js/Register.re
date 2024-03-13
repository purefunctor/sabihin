open React.Event;

[@react.component]
let make = () => {
  let (username, setUsername) = React.useState(() => "");
  let (password, setPassword) = React.useState(() => "");
  let (confirmPassword, setConfirmPassword) = React.useState(() => "");
  let (usernameError, setUsernameError) = React.useState(() => None);

  let onUsernameChange = event => {
    let value = Form.target(event)##value;
    if (String.length(value) > 0) {
      switch (Validate.Username.validate(value)) {
      | Error(e) => setUsernameError(_ => Some(e))
      | Ok(_) => setUsernameError(_ => None)
      };
    } else {
      setUsernameError(_ => None);
    };
    setUsername(_ => value);
  };
  let onPasswordChange = event => {
    setPassword(_ => Form.target(event)##value);
  };
  let onConfirmPasswordChange = event => {
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

  let usernameErrorText =
    Belt.Option.map(usernameError, Validate.Username.to_string);

  <AuthCore
    register=true
    username
    onUsernameChange
    password
    onPasswordChange
    confirmPassword
    onConfirmPasswordChange
    onSubmit
    usernameErrorText
  />;
};
