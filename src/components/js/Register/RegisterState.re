type t = {
  username: string,
  setUsername: (string => string) => unit,
  password: string,
  setPassword: (string => string) => unit,
  confirmPassword: string,
  setConfirmPassword: (string => string) => unit,
};

let use = () => {
  let (username, setUsername) = React.useState(() => "");
  let (password, setPassword) = React.useState(() => "");
  let (confirmPassword, setConfirmPassword) = React.useState(() => "");
  {
    username,
    setUsername,
    password,
    setPassword,
    confirmPassword,
    setConfirmPassword,
  };
};

type step_t =
  | Initial
  | Generate
  | Finish;

let useStep = () => {
  React.useReducer(
    (step: step_t, ()) => {
      switch (step) {
      | Initial => Generate
      | Generate => Finish
      | Finish => raise(Failure("No more states!"))
      }
    },
    Initial,
  );
};
