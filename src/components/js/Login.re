open React.Event;

[@react.component]
let make = () => {
  let (username, setUsername) = React.useState(() => "");
  let (password, setPassword) = React.useState(() => "");
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

  let usernameErrorText =
    Belt.Option.map(usernameError, Validate.Username.to_string);

  <AuthCore
    register=false
    username
    onUsernameChange
    password
    onPasswordChange
    onSubmit
    usernameErrorText
  />;
};
