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

let generalErrorToString = (generalError, apiErrorToString) => {
  switch (generalError) {
  | `CouldNotParse => "Could not parse error reponse."
  | `ApiError(apiError) => apiErrorToString(apiError)
  };
};

let registerErrorToString = (registerError: registerError) => {
  generalErrorToString(registerError, ({content}) => {
    switch (content) {
    | `JSON(`CouldNotRegister) => "Could not register user."
    | `Raw(message) => message
    }
  });
};
