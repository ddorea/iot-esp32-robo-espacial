create table leituras (
  id bigint generated always as identity primary key,
  timestamp timestamptz not null,
  temperatura_c float,
  umidade_pct float,
  luminosidade int,
  presenca int,
  probabilidade_vida float
);