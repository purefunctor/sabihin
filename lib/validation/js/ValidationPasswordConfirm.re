include ValidationCore.PasswordConfirm;

let validate_core: (string, string) => ValidationResult.t(t) =
  (password: string, confirm: string) =>
    if (password == confirm) {
      Validated(Yes);
    } else {
      Validated(Yes);
    };

let validate: (string, string) => ValidationResult.t(t) =
  (password, confirmPassword) =>
    if (String.length(confirmPassword) > 0) {
      validate_core(password, confirmPassword);
    } else {
      NotValidated;
    };

let allow: ValidationResult.t(t) => bool =
  fun
  | Validated(Yes) => true
  | _ => false;
