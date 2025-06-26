# 🗺️ CARTOGRAPHIE COMPLÈTE DES EXIT_STATUS - MINISHELL

## 📋 RÉSUMÉ DES CODES DE SORTIE IDENTIFIÉS

### ✅ Codes actuellement implémentés :
- **0** : Succès
- **1** : Erreur générale (redirections, fork, etc.)
- **2** : Argument numérique requis (exit avec argument non-numérique)
- **126** : Is a directory (tentative d'exécution d'un répertoire)
- **127** : Command not found (commande introuvable)
- **130** : SIGINT (Ctrl+C)
- **131** : SIGQUIT (Ctrl+\)
- **128 + signal** : Terminaison par signal

## 📍 LOCALISATION PAR FICHIER

### 1. `/src/prompt/prompt.c`
```c
// Ligne 86 et 126 : Gestion SIGINT
shell->exit_status = 130;
```
**Contexte** : Signal Ctrl+C pendant l'exécution ou en attente de commande

### 2. `/src/exec/pipes.c`
```c
// Ligne 60-62 : Récupération du statut des processus enfants
if (WIFEXITED(status))
    shell->exit_status = WEXITSTATUS(status);
else if (WIFSIGNALED(status))
    shell->exit_status = 128 + WTERMSIG(status);

// Ligne 65-67 : Messages pour signaux spécifiques
if (shell->exit_status == 130)      // SIGINT
    write(2, "\n", 1);
else if (shell->exit_status == 131) // SIGQUIT
    write(2, "Quit (core dumped)\n", 19);

// Lignes 148, 159, 182 : Erreurs de pipe/fork
shell->exit_status = 1;
```

### 3. `/src/exec/checks_full_cmd.c`
```c
// Ligne 82 : Commande vide
*status = 127;

// Ligne 89 : Répertoire au lieu de commande
*status = 126;

// Ligne 98 : Commande introuvable
*status = 127;
```

### 4. `/src/exec/exec_single_cmd.c`
```c
// Ligne 74 : Erreur de redirection
shell->exit_status = 1;

// Ligne 78 : Retour de builtin
shell->exit_status = handle_builtin(shell, cmd, STDOUT_FILENO);
```

### 5. `/src/builtins/builtin_exit.c`
```c
// Ligne 48 : Argument non-numérique pour exit
exit(2);

// Ligne 54 : Calcul du code de sortie
exit_code = ft_atoi(cmd->args[1]) % 256;
```

### 6. `/src/utils/error_management.c`
```c
// Fonction utilitaire pour extraire le statut
int get_exit_status(int status)
{
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (1);
}
```

## 🔍 CODES DE SORTIE MANQUANTS OU INCOMPLETS

### Codes standards Bash non implémentés :
- **125** : Command invoked cannot execute
- **128** : Invalid argument to exit
- **255** : Exit status out of range

### Codes d'erreur système potentiels :
- **EACCES (13)** → 126 : Permission denied
- **ENOENT (2)** → 127 : No such file or directory
- **EISDIR (21)** → 126 : Is a directory

## 🚨 PROBLÈMES IDENTIFIÉS

### 1. **Incohérences dans la gestion des erreurs de redirection**
- Certaines fonctions utilisent `exit(1)` directement
- D'autres modifient `shell->exit_status = 1`
- Manque d'uniformité

### 2. **Gestion des signaux incomplète**
- SIGINT (130) et SIGQUIT (131) bien gérés
- Autres signaux utilisent formule générique `128 + signal`
- Pourrait nécessiter des cas spéciaux pour d'autres signaux

### 3. **Codes de sortie des builtins**
- `exit` : codes 0, 1, 2 bien gérés
- Autres builtins : statuts à vérifier
- `cd`, `export`, `unset`, etc. pourraient avoir des codes spécifiques

### 4. **Gestion des pipes**
- Le dernier processus du pipeline détermine l'exit_status
- Comportement conforme à Bash ✅

## 📊 STATISTIQUES D'USAGE

| Code | Occurrences | Contextes principaux |
|------|-------------|---------------------|
| 0    | Implicite   | Succès par défaut |
| 1    | 6+          | Erreurs générales |
| 2    | 1           | Exit argument invalide |
| 126  | 1           | Is a directory |
| 127  | 2           | Command not found |
| 130  | 3           | SIGINT (Ctrl+C) |
| 131  | 1           | SIGQUIT (Ctrl+\) |

## 🎯 RECOMMANDATIONS D'UNIFICATION

### 1. **Créer des constantes**
```c
#define EXIT_SUCCESS 0
#define EXIT_GENERAL_ERROR 1
#define EXIT_MISUSE 2
#define EXIT_CANNOT_EXECUTE 126
#define EXIT_COMMAND_NOT_FOUND 127
#define EXIT_SIGINT 130
#define EXIT_SIGQUIT 131
```

### 2. **Fonction centralisée de gestion d'erreur**
```c
void set_exit_status(t_shell *shell, int code, const char *context);
```

### 3. **Vérifications à effectuer**
- [ ] Tous les builtins retournent des codes cohérents
- [ ] Gestion uniforme des erreurs de redirection
- [ ] Messages d'erreur standardisés
- [ ] Tests avec tous les codes de sortie

## 🧪 TESTS RECOMMANDÉS

```bash
# Tester tous les codes identifiés
./minishell -c "unknown_command"     # 127
./minishell -c "mkdir test_dir && test_dir"  # 126
./minishell -c "exit hello"          # 2
# Ctrl+C pendant exécution           # 130
# Ctrl+\ pendant exécution           # 131
```

---
*Cartographie générée le 26 juin 2025*
*Projet : Minishell - École 42*
