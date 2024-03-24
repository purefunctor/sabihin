let useMemoPrevious = (initial, compute, dependencies) => {
  let (previousValue, setPreviousValue) = React.useState(() => initial);
  React.useEffect1(
    () => {
      setPreviousValue(previousValue => compute(previousValue));
      None;
    },
    dependencies,
  );
  previousValue;
};
