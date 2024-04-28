open Types_universal.Definitions_t;

type generalError('apiError) = [ | `CouldNotParse | `ApiError('apiError)];

type registerError = generalError(register_error_response);
type registerResult = result(register_response, registerError);

let tryParse = (parser, json) => {
  switch (parser(json)) {
  | result => Ok(result)
  | exception _ => Error(`CouldNotParse)
  };
};

let tryParseError = (parser, json) => {
  switch (parser(json)) {
  | result => Error(`ApiError(result))
  | exception _ => Error(`CouldNotParse)
  };
};
