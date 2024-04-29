module Username = {
  type t =
    | ApiError(string)
    | TooShort
    | TooLong
    | InvalidCharacter(string)
    | Success;
};

module Password = {
  type t =
    | TooShort
    | TooWeak
    | VeryWeak
    | Medium
    | ModeratelyStrong
    | VeryStrong;
};

module PasswordConfirm = {
  type t =
    | Yes
    | No;
};
