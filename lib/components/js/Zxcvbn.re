type t = {score: int};

[@mel.module "zxcvbn"]
external zxcvbn_js: (string, array(string)) => t = "default";

let zxcvbn = (~userInput: array(string)=[||], password: string): t => {
  zxcvbn_js(password, userInput);
};
