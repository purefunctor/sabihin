open React.Event;

[@react.component]
let make = () => {
  let (username, setUsername) = React.useState(() => "");
  let (password, setPassword) = React.useState(() => "");
  let (confirmPassword, setConfirmPassword) = React.useState(() => "");

  let usernameState =
    React.useMemo1(
      _ =>
        if (String.length(username) > 0) {
          Some(Validate.Username.validate(username));
        } else {
          None;
        },
      [|username|],
    );
  let passwordState =
    React.useMemo2(
      _ => {
        let passwordState = ref(None);
        if (String.length(password) > 0) {
          passwordState :=
            Some(Validate.Password.validate(username, password));
        };
        passwordState^;
      },
      (username, password),
    );
  let confirmState =
    React.useMemo2(
      _ => {
        let confirmState = ref(None);
        if (String.length(confirmPassword) > 0) {
          confirmState :=
            Some(Validate.Confirm.validate(password, confirmPassword));
        };
        confirmState^;
      },
      (password, confirmPassword),
    );

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

    let formData = Fetch.FormData.make();
    Fetch.FormData.append("username", username, formData);
    Js.Console.log(formData);

    let _ =
      Js.Promise.(
        Fetch.fetchWithInit(
          "/api/register",
          Fetch.RequestInit.make(
            ~method_=Post,
            ~body=Fetch.BodyInit.makeWithFormData(formData),
            (),
          ),
        )
        |> then_(Fetch.Response.text)
        |> then_(json => json |> print_endline |> resolve)
      );
    ();
  };

  <main className="login-register-content">
    <AuthForm
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
    />
  </main>;
};
