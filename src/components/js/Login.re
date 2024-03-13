open React.Event;

[@react.component]
let make = () => {
  let (username, setUsername) = React.useState(() => "");
  let (password, setPassword) = React.useState(() => "");

  let onUsernameChange = event => {
    setUsername(_ => React.Event.Form.target(event)##value);
  };
  let onPasswordChange = event => {
    setPassword(_ => React.Event.Form.target(event)##value);
  };

  let onSubmit = event => {
    Form.preventDefault(event);
    let username = Form.target(event)##username##value;
    let password = Form.target(event)##password##value;
    Js.Console.log2("Username:", username);
    Js.Console.log2("Password:", password);
  };

  <AuthCore
    register=false
    username
    onUsernameChange
    password
    onPasswordChange
    onSubmit
  />;
};
