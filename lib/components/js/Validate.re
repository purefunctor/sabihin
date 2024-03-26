module Username = {
  open AuthForm.Validated;
  open AuthForm.UsernameState;

  let disallow_pattern = [%re "/[^A-Za-z0-9\\._]/g"];

  let validateCore = (username: string) => {
    let error = ref(None);
    if (String.length(username) <= 3) {
      error := Some(TooShort);
    } else if (String.length(username) >= 16) {
      error := Some(TooLong);
    } else {
      error :=
        username
        ->Js.String.match(~regexp=disallow_pattern)
        ->Belt.Option.flatMap(disallow_match =>
            disallow_match->Belt.Array.get(0)
          )
        ->Belt.Option.flatMap(invalid =>
            invalid->Belt.Option.map(invalid => InvalidCharacter(invalid))
          );
    };
    switch (error^) {
    | None => Success
    | Some(error) => error
    };
  };

  let validate = (username: string) =>
    if (String.length(username) > 0) {
      Validated(validateCore(username));
    } else {
      NotValidated;
    };
};

module Password = {
  open AuthForm.Validated;
  open AuthForm.PasswordState;

  let validateCore = (username: string, password: string) =>
    if (String.length(password) < 8) {
      TooShort;
    } else {
      let result = Zxcvbn.zxcvbn(~userInput=[|username|], password);
      switch (result.score) {
      | 0 => TooWeak
      | 1 => VeryWeak
      | 2 => Medium
      | 3 => ModeratelyStrong
      | 4 => VeryStrong
      | _ => raise(Failure("Invalid state from zxcvbn"))
      };
    };

  let validate = (username: string, password: string) =>
    if (String.length(password) > 0) {
      Validated(validateCore(username, password));
    } else {
      NotValidated;
    };
};

module Confirm = {
  open AuthForm.Validated;
  open AuthForm.ConfirmState;

  let validateCore = (password: string, confirm: string) =>
    if (password == confirm) {
      Yes;
    } else {
      No;
    };

  let validate = (password, confirmPassword) =>
    if (String.length(confirmPassword) > 0) {
      Validated(validateCore(password, confirmPassword));
    } else {
      NotValidated;
    };
};
