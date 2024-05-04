include ValidationCore.PasswordBasic;

let validate_core: string => ValidationResult.t(t) =
  password =>
    if (String.length(password) < 8) {
      Validated(TooShort);
    } else {
      Validated(Success);
    };

let validate: string => ValidationResult.t(t) =
  password =>
    if (String.length(password) > 0) {
      validate_core(password);
    } else {
      NotValidated;
    };

let allow: ValidationResult.t(t) => bool =
  fun
  | Validated(TooShort) => false
  | Validated(Success) => true
  | NotValidated => false;
