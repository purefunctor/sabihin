open React.Event;

type field_hook('kind) = {
  value: string,
  onChange: Form.t => unit,
  validation: ValidationResult.t('kind),
};
