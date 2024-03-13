open React.Event;

[@react.component]
let make = () => {
  let (username, setUsername) = React.useState(() => "");
  let (password, setPassword) = React.useState(() => "");
  let (confirmPassword, setConfirmPassword) = React.useState(() => "");

  let onUsernameChange = event => {
    setUsername(_ => Form.target(event)##value);
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

  <AuthCore
    register=true
    username
    onUsernameChange
    password
    onPasswordChange
    confirmPassword
    onConfirmPasswordChange
    onSubmit
  />;
};
