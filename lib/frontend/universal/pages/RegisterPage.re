[@react.component]
let make = () => {
  let username = RegisterPageHooks.useUsername();
  let password = RegisterPageHooks.usePassword(~username=username.value);
  let confirm = RegisterPageHooks.useConfirm(~password=password.value);

  let stage = RegisterPageHooks.useStage();
  let RegisterPageHooksCore.{current, toGenerate, toSubmit} = stage;

  let onSubmit =
    RegisterPageHooks.useFormSubmit(
      ~username,
      ~password,
      ~confirm,
      ~toGenerate,
    );

  React.useEffect0(() => {
    toGenerate(Obj.magic(None));
    None;
  });

  <RegisterPageLayout>
    {switch (current) {
     | Form => <RegisterPageForm username password confirm onSubmit />
     | Generate(generate) => <RegisterPageGenerate generate toSubmit />
     | Submit(_) => React.null
     }}
  </RegisterPageLayout>;
};
