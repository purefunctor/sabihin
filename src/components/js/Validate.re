module Username = {
  type t =
    | TooShort
    | TooLong
    | InvalidCharacter(string);

  let to_string =
    fun
    | TooShort => "Must be more than 3 characters."
    | TooLong => "Must be less than 16 characters."
    | InvalidCharacter(c) =>
      Printf.sprintf("'%s' is not a valid character.", c);

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
    | None => Ok(username)
    | Some(error) => Error(error)
    };
  };
};
