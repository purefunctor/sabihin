open React.Event;

[@react.component]
let make = () => {
  let (username, setUsername) = React.useState(() => "");
  let (password, setPassword) = React.useState(() => "");

  let onUsernameChange = event => {
    let value = Form.target(event)##value;
    setUsername(_ => value);
  };
  let onPasswordChange = event => {
    let value = Form.target(event)##value;
    setPassword(_ => value);
  };

  let onSubmit = event => {
    Form.preventDefault(event);
    let username = Form.target(event)##username##value;
    let password = Form.target(event)##password##value;
    Js.Console.log2("Username:", username);
    Js.Console.log2("Password:", password);
  };

  <main className="login-register-content">
    <AuthForm
      register=false
      username
      onUsernameChange
      password
      onPasswordChange
      onSubmit
    />
  </main>;
};
