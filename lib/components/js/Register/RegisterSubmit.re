[@react.component]
let make = (~submit: RegisterHooks.Step.submit) => {
  React.string(submit.publicId);
};
