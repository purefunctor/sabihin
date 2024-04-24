open React.Event;

type fieldHook('kind) = {
  value: string,
  onChange: Form.t => unit,
  validation: ValidationResult.t('kind),
};

