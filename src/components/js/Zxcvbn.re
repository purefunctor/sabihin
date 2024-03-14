type t = {score: int};

[@mel.module "zxcvbn"]
external zxcvbn_js: (string, array(string)) => t = "default";

let zxcvbn = (~userInput: option(array(string))=?, password: string): t => {
  let userInput = Option.value(userInput, ~default=[||]);
  zxcvbn_js(password, userInput);
};
