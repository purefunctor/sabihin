open React.Event;

type fieldHook('kind) = {
  value: string,
  onChange: Form.t => unit,
  validation: ValidationResult.t('kind),
};

type t('generate, 'submit) =
  | Form
  | Generate('generate)
  | Submit('submit);

type hook('generate, 'submit) = {
  current: t('generate, 'submit),
  toGenerate: 'generate => unit,
  toSubmit: 'submit => unit,
};
