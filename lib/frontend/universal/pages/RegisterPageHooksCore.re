open React.Event;

type fieldHook('kind) = {
  value: string,
  onChange: Form.t => unit,
  validation: ValidationResult.t('kind),
};

type stage =
  | Form
  | Generate
  | Submit;

type stageHook = {
  current: stage,
  toGenerate: unit => unit,
  toSubmit: unit => unit,
};
