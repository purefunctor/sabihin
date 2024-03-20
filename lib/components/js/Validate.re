module Username = {
  open AuthForm.UsernameState;

  let disallow_pattern = [%re "/[^A-Za-z0-9\\._]/g"];

  let validate = (username: string) => {
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
};

module Password = {
  open AuthForm.PasswordState;

  let validate = (username: string, password: string) =>
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
};

module Confirm = {
  open AuthForm.ConfirmState;

  let validate = (password: string, confirm: string) =>
    if (password == confirm) {
      Yes;
    } else {
      No;
    };
};
