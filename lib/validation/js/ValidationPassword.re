include ValidationCore.Password;

let validate_core: (string, string) => ValidationResult.t(t) =
  (username, password) => {
    open ValidationZxcvbn;
    let result = ref(VeryStrong);
    if (String.length(password) < 8) {
      result := TooShort;
    } else {
      let {score} = zxcvbn(~userInput=[|username|], password);
      switch (score) {
      | 0 => result := TooWeak
      | 1 => result := VeryWeak
      | 2 => result := Medium
      | 3 => result := ModeratelyStrong
      | 4 => result := VeryStrong
      | _ => failwith("unexpected result from zxcvbn.")
      };
    };
    Validated(result^);
  };

let validate: (string, string) => ValidationResult.t(t) =
  (username, password) =>
    if (String.length(password) > 0) {
      validate_core(username, password);
    } else {
      NotValidated;
    };

let allow: ValidationResult.t(t) => bool =
  fun
  | Validated(TooShort)
  | Validated(TooWeak)
  | Validated(VeryWeak) => false
  | Validated(Medium)
  | Validated(ModeratelyStrong)
  | Validated(VeryStrong) => true
  | NotValidated => false;
