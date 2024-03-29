include ValidationCore.Username;

let disallow_pattern = [%re "/[^A-Za-z0-9\\._]/g"];

let validate_core: string => ValidationResult.t(t) =
  username => {
    let result = ref(Success);
    if (String.length(username) <= 3) {
      result := TooShort;
    } else if (String.length(username) >= 16) {
      result := TooLong;
    } else {
      username
      ->Js.String.match(~regexp=disallow_pattern)
      ->Belt.Option.flatMap(disallow_match =>
          disallow_match->Belt.Array.get(0)
        )
      ->Belt.Option.forEach(invalid =>
          invalid->Belt.Option.forEach(invalid =>
            result := InvalidCharacter(invalid)
          )
        );
    };
    Validated(result^);
  };

let validate: string => ValidationResult.t(t) =
  username =>
    if (String.length(username) > 0) {
      validate_core(username);
    } else {
      NotValidated;
    };
